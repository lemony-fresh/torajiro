#include <iostream>
#include <vector>

// must be included first
#include "SgSystem.h"

#include "SgInit.h"
#include "SgNode.h"
#include "GoGame.h"
#include "GoInit.h"

#include "sgfreader.h"

int main(int argc, char** argv){


    /* initialize libs */
    SgInit();
    GoInit();

    // for testing
    std::string database_path = (argc >= 2 ? std::string(argv[1]) : std::string("../../home/go_bachelor_thesis/data/gamesets/9x9/from_gobase"));
    std::vector<GoGame*> games = read_games(database_path);

    GoGame* game = games.at(0);

    std::cout << "board size: " << game->Board().Size() << std::endl;

    std::vector<SgMove> gameline;
    while(true){
        std::cout << game->Board() << std::endl;

        SgMove move = game->CurrentMove();
        if(!SgIsSpecialMove(move))
            gameline.push_back(game->CurrentMove());

        if(!game->CanGoInDirection(SgNode::Direction::NEXT))
            break;
        game->GoInDirection(SgNode::Direction::NEXT);
    }

    // TODO check that reading the other files worked as well

    // all the games have been created with new so they must be deleted here
    for (std::size_t i = 0; i < games.size(); ++i)
        delete games.at(i);
    games.clear();

    SgFini();
    GoFini();

    return 0;
}
