#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <utility>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// must be included first
#include "SgSystem.h"

#include "SgGameReader.h"
#include "GoNodeUtil.h"
#include "GoGame.h"
#include "GoBoard.h"
#include "GoPlayerMove.h"

#include "bitboard.h"

/**
 * @return true iff the given path ends with ".sgf"
 */
bool ends_with_sgf(const boost::filesystem::path& s) {
    std::string extension(".sgf");
    return extension.compare(s.extension().string()) == 0;
}

/**
 * @brief read_games reads all go games below the given path
 * @param path if path is a file, only this file is read, otherwise all sgf files below this path are read.
 * @return
 */
template <int N>
std::vector<std::pair<std::shared_ptr<Bitboard<N> >, GoPlayerMove> >
read_games(const std::string &path) {
    std::vector<std::pair<std::shared_ptr<Bitboard<N> >,GoPlayerMove> > moves;

    /* find all sgf files */

    std::vector<boost::filesystem::path> all_files;
    boost::filesystem::path p(path);
    if (!boost::filesystem::exists(p) || (!boost::filesystem::is_regular_file(p) && !boost::filesystem::is_directory(p)))
        return moves;
    if (boost::filesystem::is_regular_file(p) && ends_with_sgf(p)) { // p is a file
        all_files.push_back(p);
    } else { // p is a folder
        boost::filesystem::recursive_directory_iterator it(boost::filesystem::absolute(p)), end;
        for(; it != end; it++)
            if(boost::filesystem::is_regular_file(it->path()) && ends_with_sgf(it->path()))
                all_files.push_back(it->path());
    }

    std::sort(all_files.begin(), all_files.end());

    std::cout << "I found " << all_files.size() << " .sgf files." << std::endl;

    /* read all sgf files */
//#pragma omp parallel for
    for(int i = 0; i < all_files.size(); ++i) {
        boost::filesystem::path const& file = all_files.at(i);
        std::ifstream in(file.string());
        if (! in) { std::cerr << "Could not open file " << file.string() << std::endl; continue; }
        SgGameReader reader(in);
        SgNode* root = reader.ReadGame();
        if (root == 0) { std::cerr << "No games in file " << file.string() << std::endl; continue; }
        if (reader.GetWarnings().any()){
            SgWarning() << file.string() << ":\n";
            reader.PrintWarnings(SgDebug());
        }

        /* convert node tree into game */
        std::shared_ptr<GoGame> game = std::make_shared<GoGame>();
        game->Init(root);
        GoRules rules;
        rules.SetKomi(GoNodeUtil::GetKomi(game->CurrentNode()));
        rules.SetHandicap(GoNodeUtil::GetHandicap(game->CurrentNode()));
        game->SetRulesGlobal(rules);


        if (game->Board().Size() != N) {
            std::cout << "Warning: I am skipping game " << file.string()
                      << " because its board size is " << game->Board().Size()
                      << " instead of " << N << "." << std::endl;
            continue;
        }

        /* iterate over all moves in the game */
        while(true){
            if(game->CurrentNode()->IsBranchPoint()) {
                std::cout << "Warning: I am skipping the rest of game " << file.string()
                          << " because from here on it contains branches." << std::endl;
                break;
            }
            SgMove move = game->CurrentMove();
            // push current move if it's not special (pass move or before game start)
            if(!SgIsSpecialMove(move)) {
                GoPlayerMove player_move(game->Board().ToPlay(), game->CurrentMove());
                auto board = std::make_shared<Bitboard<N> >(game->Board());
                moves.push_back(std::make_pair(board, player_move));
            }
            if(!game->CanGoInDirection(SgNode::Direction::NEXT))
                break;
            game->GoInDirection(SgNode::Direction::NEXT);
        }
    }

    return moves;
}
