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
 * @brief read_games reads all go games below the given path
 * @param path if path is a file, only this file is read, otherwise all sgf files below this path are read.
 * @return
 */
template <std::size_t N>
std::vector<std::pair<BitboardPtr<N>, GoPlayerMove>>
read_games(std::string const & path, bool verbose = true) {

    std::vector<std::pair<BitboardPtr<N>, GoPlayerMove>> moves;

    /* find all .sgf files */
    std::vector<boost::filesystem::path> all_files;
    boost::filesystem::path p(path);
    if (!boost::filesystem::exists(p) ||
            (!boost::filesystem::is_regular_file(p) && !boost::filesystem::is_directory(p)))
        return moves;
    if (boost::filesystem::is_regular_file(p) && std::string(".sgf").compare(p.extension().string()) == 0) { // p is a file and ends on ".sgf"
        all_files.push_back(p);
    } else { // p is a folder
        boost::filesystem::recursive_directory_iterator it(boost::filesystem::absolute(p)), end;
        for(; it != end; it++)
            if(boost::filesystem::is_regular_file(it->path()) && std::string(".sgf").compare(it->path().extension().string()) == 0)
                all_files.push_back(it->path());
    }

    std::sort(all_files.begin(), all_files.end());

    if (verbose)
        std::cout << all_files.size() << " sgf files found..." << std::flush;

    /* read all files */
    for(std::size_t i = 0; i < all_files.size(); ++i) {
        boost::filesystem::path const& file = all_files.at(i);
        std::ifstream in(file.string());
        if (! in) { std::cerr << "Could not open file " << file.string() << std::endl; continue; }

        SgGameReader reader(in);
        SgNode* root = reader.ReadGame();
        in.close();

        if (root == 0) { std::cerr << "No games in file " << file.string() << std::endl; continue; }
        if (reader.GetWarnings().any()){ SgWarning() << file.string() << ":\n"; reader.PrintWarnings(SgDebug()); }


        /* convert node tree into game */
        GoGame game;
        game.Init(root);
        GoRules rules;
        rules.SetKomi(GoNodeUtil::GetKomi(game.CurrentNode()));
        rules.SetHandicap(GoNodeUtil::GetHandicap(game.CurrentNode()));
        game.SetRulesGlobal(rules);

        if (game.Board().Size() != N) {
            if (verbose)
                std::cout << "Warning: I am skipping game " << file.string() << " because its board size is "
                          << game.Board().Size() << " instead of " << N << "." << std::endl;
            continue;
        }

        /* iterate over all moves in the game */
        while(true){
            if(game.CurrentNode()->IsBranchPoint()) {
                if (verbose)
                    std::cout << "Warning: I am skipping the rest of game " << file.string() << " because from move "
                              << game.CurrentMoveNumber() << " on it contains branches." << std::endl;
                break;
            }
            SgMove move = game.CurrentMove();
            // push current move if it's not special (pass move or before game start)
            if(!SgIsSpecialMove(move)) {
                SgPoint move = game.CurrentMove();
                game.GoInDirection(SgNode::Direction::PREVIOUS);
                GoPlayerMove player_move(game.Board().ToPlay(), move);
                BitboardPtr<N> board = Bitboard<N>::create(game.Board());
                moves.emplace_back(board, player_move);
                game.GoInDirection(SgNode::Direction::NEXT);
            }
            if(!game.CanGoInDirection(SgNode::Direction::NEXT))
                break;
            game.GoInDirection(SgNode::Direction::NEXT);
        }
    }

    return moves;
}
