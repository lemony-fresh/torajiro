#include <iostream>
#include <vector>
#include <string>

// must be included first
#include "SgSystem.h"

#include "SgInit.h"
#include "SgNode.h"
#include "GoGame.h"
#include "GoInit.h"

#include "sgfreader.h"

template <int N> void do_everything(std::string const& database_path) {

    /* initialize libs */
    SgInit();
    GoInit();

    /* read all stuff */
    std::vector<std::pair<std::shared_ptr<Bitboard<N> >, GoPlayerMove> > moves = read_games<N>(database_path);
    std::cout << "These games contained " << moves.size() << " moves." << std::endl;

    // rotate boards and moves properly

    // find k-th nearest stone, remove all that are further away

    // compute admissible translations

    // generate pattern (partially cleared board + move + translation info)

    // insert into big list after searching for duplicates

    // assign all patterns scores how often they matched in the training set and how often they were actually played

    // prune too infrequently used and too inaccurate patterns

    SgFini();
    GoFini();
}

int main(int argc, char** argv){

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <board size> <.sgf file or folder>\ne.g. "
                  << argv[0] << " 9 $HOME/repos/home/go_bachelor_thesis/data/gamesets/9x9/from_gobase"
                  << std::endl;
        return -1;
    }

    int const n = std::stoi(std::string(argv[1]));
    std::string const database_path = std::string(argv[2]);

    switch (n) {
    case  9: do_everything< 9>(database_path); break;
    case 13: do_everything<13>(database_path); break;
    case 19: do_everything<19>(database_path); break;
    default: throw std::invalid_argument("board size is not 9, 13 or 19");
    }

    return 0;
}
