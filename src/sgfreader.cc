#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

// must be included first
#include "SgSystem.h"

#include "SgGameReader.h"
#include "GoNodeUtil.h"

#include "GoGame.h"

bool ends_with_sgf(const boost::filesystem::path& s) {
    std::string extension(".sgf");
    return extension.compare(s.extension().string()) == 0;
}

std::vector<GoGame*> read_games(const std::string &path)
{


    std::vector<GoGame*> games;

    /* find all sgf files */

    std::vector<boost::filesystem::path> all_files;
    boost::filesystem::path p(path);
    if (!boost::filesystem::exists(p) || (!boost::filesystem::is_regular_file(p) && !boost::filesystem::is_directory(p)))
        return games;
    if (boost::filesystem::is_regular_file(p) && ends_with_sgf(p)) { // p is a file
        all_files.push_back(p);
    } else { // p is a folder
        boost::filesystem::recursive_directory_iterator it(boost::filesystem::absolute(p)), end;
        for(; it != end; it++)
            if(boost::filesystem::is_regular_file(it->path()) && ends_with_sgf(it->path()))
                all_files.push_back(it->path());
    }

    std::sort(all_files.begin(), all_files.end());

    /* read all sgf files */

    // TODO: check that all game files have the same board size etc.
    // TODO: progress output

    for(boost::filesystem::path file : all_files) {
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
        GoGame* game = new GoGame;
        game->Init(root);
        GoRules rules;
        rules.SetKomi(GoNodeUtil::GetKomi(game->CurrentNode()));
        rules.SetHandicap(GoNodeUtil::GetHandicap(game->CurrentNode()));
        game->SetRulesGlobal(rules);

        games.push_back(game);
    }

    return games;
}
