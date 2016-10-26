#pragma once

#include <vector>
#include <unordered_set>
#include <memory>
#include <utility>

#include "GoPlayerMove.h"

#include "bitboard.h"
#include "pattern.h"

int fails = 0;

template <std::size_t N>
std::vector<PatternPtr<N>>
generate_patterns(std::vector<std::pair<BitboardPtr<N>, GoPlayerMove>> const & training_set) {
    std::unordered_set<PatternPtr<N>> patterns;

    for (std::pair<BitboardPtr<N>, GoPlayerMove> const & training_example : training_set) {
        Bitboard<N> board(*(training_example.first)); // make copy so we can freely modify it
        SgPoint move = training_example.second.Point();

        tora_equals(training_example.second.Color(), SG_BLACK);
        tora_equals(board.get(move), SG_EMPTY);
        if (board.get(move) != SG_EMPTY) {
            ++fails;
            std::cout << board << std::endl;
            std::cout << SgPointUtil::Col(move) << "," << SgPointUtil::Row(move) << std::endl;
            std::cout << board.get(move) << std::endl;
            if(fails > 10) std::exit(1);
        }

        std::size_t const max_diamond_radius = 10;

        std::bitset<N*N> mask;
        mask.flip();
        tora_assert(mask.all()); // assert that the mask is true everywhere

        for (int diamond_radius = max_diamond_radius; diamond_radius >= 1; --diamond_radius) {
            for (std::size_t y = 1; y <= N; ++y) {
                for (std::size_t x = 1; x <= N; ++x) {
                    SgPoint p = SgPointUtil::Pt(x, y);
                    if(SgPointUtil::Distance(p, move) > diamond_radius) {
                        board.set(p, SG_EMPTY);
                        mask[util::twoDto1D<N>(x - 1, y - 1)] = false;
                    }
                }
            }

            int const move_x = SgPointUtil::Col(move),
                      move_y = SgPointUtil::Row(move),
                      min_x_translation = (move_x - diamond_radius <= 1      ? 0 : -diamond_radius),
                      max_x_translation = (move_x + diamond_radius >= (int)N ? 0 :  diamond_radius),
                      min_y_translation = (move_y - diamond_radius <= 1      ? 0 : -diamond_radius),
                      max_y_translation = (move_y + diamond_radius >= (int)N ? 0 :  diamond_radius);

            // TODO think about whether it's possible to generate large patterns at the border/corner
            // TODO should we do the board normalization after the whole translating stuff?

            Pattern<N> pat(board, mask, move, max_x_translation - min_x_translation, max_y_translation - min_y_translation);
            // shift to the minimal position so we don't need the min_x/y_translation anymore.
            pat.translate(min_x_translation, min_y_translation);
            // insert and account for duplicates
            patterns.insert(std::make_shared<Pattern<N>>(pat));

            // if the diamond touches a vertical border, we also create a pattern with no x translation assuming that
            // the border may be an important cue and a pattern with no translation is more accurate in the evaluation
            if (min_x_translation == 0 || max_x_translation == 0) {
                Pattern<N> pat(board, mask, move, 0, max_y_translation - min_y_translation);
                pat.translate(0, min_y_translation);
                patterns.insert(std::make_shared<Pattern<N>>(pat));
            }
            // same for the horizontal border
            if (min_y_translation == 0 || max_y_translation == 0) {
                Pattern<N> pat(board, mask, move, max_x_translation - min_x_translation, 0);
                pat.translate(min_x_translation, 0);
                patterns.insert(std::make_shared<Pattern<N>>(pat));
            }

        }
    }

    std::vector<PatternPtr<N>> result(patterns.begin(), patterns.end());
    return result;
}
