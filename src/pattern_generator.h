#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "GoPlayerMove.h"

#include "bitboard.h"
#include "pattern.h"

template <std::size_t N>
std::vector<PatternPtr<N>>
generate_patterns(std::vector<std::pair<BitboardPtr<N>, GoPlayerMove>> const & training_set) {
    std::vector<PatternPtr<N>> patterns;

    for (std::pair<BitboardPtr<N>, GoPlayerMove> const & training_example : training_set) {
        assert(training_example.second.Player() == SG_BLACK);
        // TODO
        // - find k-th nearest stone, remove all that are further away
        // - create diamond shaped mask with radious to k-th nearest stone
        // - compute admissible translations
        // - generate pattern (partially cleared board + move + translation info)
        // - search for duplicates and insert into big list
    }

    return patterns;
}
