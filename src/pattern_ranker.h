#pragma once

#include <vector>
#include <utility>

#include "GoPlayerMove.h"

#include "pattern.h"

template <std::size_t N>
std::vector<std::pair<PatternPtr<N>, float>>
rank_patterns
(std::vector<PatternPtr<N>> const & patterns, std::vector<std::pair<BitboardPtr<N>, GoPlayerMove>> const & training_set) {
    std::map<PatternPtr<N>, std::pair<std::size_t, std::size_t>> pattern_matches_and_plays;

    // insert all patterns into the map (if they are not yet
    // in it) and increment the match and the play counter
#pragma omp parallel for schedule(dynamic)
    for (std::size_t i = 0; i < patterns.size(); ++i) {
#pragma omp critical
        if(i % 100 == 0) std::cout << "\rRanking pattern " << i << " of " << patterns.size() << "..." << std::flush;
        PatternPtr<N> pattern = patterns[i];
        auto to_insert = std::make_pair(pattern, std::make_pair(std::size_t(0), std::size_t(0)));
        auto position = pattern_matches_and_plays.insert(to_insert).first; // this only inserts if it's not yet in there
        for (std::pair<BitboardPtr<N>, GoPlayerMove> const & training_ex : training_set) {
            SgPoint p = pattern->match(*(training_ex.first), training_ex.second.Color());
            if (p != SG_PASS) {// the pattern matched
                position->second.first++;
                if (p == training_ex.second.Point()) // the matching point is identical with the play
                    position->second.second++;
            }
        }
    }
    std::cout << "finished." << std::endl;

    // compute accuracy and insert into result list
    std::cout << "Computing accuracy..." << std::flush;
    float const min_relative_play_frequency = 0.0001f;
    float const min_accuracy = 0.25f;
    std::vector<std::pair<PatternPtr<N>, float>> result;
    for (auto const & evaluated_pattern : pattern_matches_and_plays) {
        float relative_play_frequency = evaluated_pattern.second.second / training_set.size();
        float accuracy = evaluated_pattern.second.second / evaluated_pattern.second.first;
        if (relative_play_frequency >= min_relative_play_frequency && accuracy >= min_accuracy)
            result.emplace_back(std::make_pair(evaluated_pattern.first, accuracy));
    }

    // sort by descending accuracy
    std::sort(result.begin(), result.end(),
        [] (std::pair<PatternPtr<N>, float> const & a, std::pair<PatternPtr<N>, float> const & b)
            {return a.second > b.second;}
    );
    std::cout << std::endl;

    return result;
}
