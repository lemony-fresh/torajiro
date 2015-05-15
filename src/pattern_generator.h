#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "GoPlayerMove.h"

#include "bitboard.h"
#include "pattern.h"

template <std::size_t N>
std::vector<PatternPtr<N>>
generate_patterns(std::vector<std::pair<BitboardPtr<N>, GoPlayerMove>> const & moves) {
    std::vector<PatternPtr<N>> patterns;

    // find k-th nearest stone, remove all that are further away

    // compute admissible translations

    // generate pattern (partially cleared board + move + translation info)

    // search for duplicates and insert into big list

    return patterns;
}
