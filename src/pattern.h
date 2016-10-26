#pragma once

#include "SgPoint.h"
#include "SgBWArray.h"

#include "bitboard.h"
#include "util.h"

template <std::size_t N> class Pattern;

template <std::size_t N> using PatternPtr = std::shared_ptr<Pattern<N> >;

// N defines the board size (e.g., 9 or 19)
template <std::size_t N>
class Pattern
{
private:

    Bitboard<N> stones;
    std::bitset<N*N> mask;
    SgPoint move;
    int max_x_translation, max_y_translation;

public:

    Pattern(Bitboard<N> const & board,
            std::bitset<N*N> const & mask,
            SgPoint const & move,
            int max_x_translation,
            int max_y_translation)
        : stones(Bitboard<N>(board)), mask(mask), move(move),
          max_x_translation(max_x_translation), max_y_translation(max_y_translation) {
        // just to be sure, we mask out everything in the bitboard that is 0 in the mask
        for (SgBWIterator it; it; ++it)
            stones.bw_bitboards[*it] &= mask;
    }

    /**
     * Short-hand to directly construct a new
     * Pattern and obtain a shared pointer to it.
     */
    template <typename... Args>
    static PatternPtr<N> create(Args&&... args) {
        return std::make_shared<Pattern<N> >(std::forward<Args>(args)...);
    }

    bool operator==(Pattern<N> const & rhs) {
        return max_x_translation == rhs.max_x_translation &&
               max_y_translation == rhs.max_y_translation &&
               stones == rhs.stones &&
               mask == rhs.mask &&
               move == rhs.move;
    }
    bool operator!=(Pattern<N> const & rhs){ return !(this == rhs); }

    /**
     * @param board
     * @param player the player whose turn it is
     * @return the point where this pattern matches a given board (or SG_PASS if it does not match at all)
     */
    SgPoint match(Bitboard<N> const & board, SgBlackWhite const & to_move) {
        Bitboard<N> copy(board);

        // all patterns are from black's point of view
        // so we invert the board if white is to move
        if (to_move == SG_WHITE)
            copy.invert();

        // compute all 8 rotations
        std::vector<Bitboard<N> > board_rotations;
        board_rotations.emplace_back(copy);
        copy.flip_horizontally();
        board_rotations.emplace_back(copy);
        copy.flip_vertically();
        board_rotations.emplace_back(copy);
        copy.flip_horizontally();
        board_rotations.emplace_back(copy);
        copy.flip_on_secondary_diagonal();
        board_rotations.emplace_back(copy);
        copy.flip_vertically();
        board_rotations.emplace_back(copy);
        copy.flip_horizontally();
        board_rotations.emplace_back(copy);
        copy.flip_vertically();
        board_rotations.emplace_back(copy);

        for (int dy = 0; dy <= max_y_translation; ++dy) {
            for (int dx = 0; dx <= max_x_translation; ++dx) {
                for(Bitboard<N> const & rotated_board : board_rotations) {
                    // if they match
                    if ((rotated_board.bw_bitboards[SG_BLACK] & mask) == stones.bw_bitboards[SG_BLACK] &&
                        (rotated_board.bw_bitboards[SG_WHITE] & mask) == stones.bw_bitboards[SG_WHITE]) {
                        translate(-dx, -dy); // translate back into original position
                        return SgPoint(move);
                    }
                }
                translate(1, 0);
            }
            // the pattern is in x-position max_x_translation + 1
            // so we translate it back to its origin and then one row down
            translate(-max_x_translation - 1, 1);
        }
        // the pattern is in position 0, max_y_translation+1
        // and we translate it back to the origin
        translate(0, -max_y_translation - 1);

        return SG_PASS;
    }

    void translate(int dx, int dy) {
        int translation = util::twoDto1D<N>(dx, dy);
        if (translation > 0) {
            mask >>= translation;
            for (SgBWIterator it; it; ++it)
                stones.bw_bitboards[*it] >>= translation;
        } else if (translation < 0) {
            mask <<= translation;
            for (SgBWIterator it; it; ++it)
                stones.bw_bitboards[*it] <<= translation;
        }
        move += dx * SG_WE + dy * SG_NS;
    }

};
