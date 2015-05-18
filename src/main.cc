#include <iostream>
#include <vector>
#include <string>

// must be included first
#include "SgSystem.h"

#include "SgInit.h"
#include "SgNode.h"
#include "GoGame.h"
#include "GoInit.h"

#include "sgf_reader.h"
#include "move_normalizer.h"
#include "pattern.h"
#include "pattern_generator.h"
#include "pattern_ranker.h"

template <std::size_t N> void do_everything(std::string const& database_path) {

    SgInit();
    GoInit();

    std::cout << "Reading all sgf games..." << std::flush;
    std::vector<std::pair<BitboardPtr<N>, GoPlayerMove>> training_set = read_games<N>(database_path);
    std::cout << training_set.size() << " moves found." << std::endl;

    std::cout << "Normalizing all moves..." << std::flush;
    normalize_all_moves(training_set);
    std::cout << "done." << std::endl;

    std::cout << "Generating patterns..." << std::flush;
    std::vector<PatternPtr<N>> patterns = generate_patterns<N>(training_set);
    std::cout << patterns.size() << " patterns generated." << std::endl;

    std::vector<std::pair<PatternPtr<N>, float>> ranked_patterns = rank_patterns(patterns, training_set);

    // TODO evaluate prediction accuracy with test set

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

    std::size_t const n = std::stoi(std::string(argv[1]));
    std::string const database_path = std::string(argv[2]);

    switch (n) {
    case  9: do_everything< 9>(database_path); break;
    case 13: do_everything<13>(database_path); break;
    case 19: do_everything<19>(database_path); break;
    default: throw std::invalid_argument("board size is not 9, 13 or 19");
    }

    return 0;
}
