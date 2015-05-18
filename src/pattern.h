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

    int min_x_translation, max_x_translation, min_y_translation, max_y_translation;
    Bitboard<N> stones;
    std::bitset<N*N> mask;
    SgPoint move;

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

public:

    /**
     * Short-hand to directly construct a new
     * Pattern and obtain a shared pointer to it.
     */
    template <typename... Args>
    static PatternPtr<N> create(Args&&... args) {
        return std::make_shared<Pattern<N> >(std::forward<Args>(args)...);
    }

    /**
     * @param board
     * @param player the player whose turn it is
     * @return the point where this pattern matches a given board (or SG_PASS if it does not match at all)
     */
    SgPoint match(Bitboard<N> const & board, SgBlackWhite const & to_move) {
        Bitboard<N> copy(board);
        // all patterns are from black's point of view
        // so we need to invert if it is white's move
        if (to_move == SG_WHITE)
            copy.invert();

        // compute all 8 rotations
        std::vector<Bitboard<N> > rotations;
        rotations.emplace_back(copy);
        copy.flip_horizontally();
        rotations.emplace_back(copy);
        copy.flip_vertically();
        rotations.emplace_back(copy);
        copy.flip_horizontally();
        rotations.emplace_back(copy);
        copy.flip_on_secondary_diagonal();
        rotations.emplace_back(copy);
        copy.flip_vertically();
        rotations.emplace_back(copy);
        copy.flip_horizontally();
        rotations.emplace_back(copy);
        copy.flip_vertically();
        rotations.emplace_back(copy);

        translate(min_x_translation, min_y_translation);
        for (int dy = min_y_translation; dy <= max_y_translation; ++dy, translate(0, 1))
            for (int dx = min_x_translation; dx <= max_x_translation; ++dx, translate(1, 0))
                // if they match
                if (   (board.bw_bitboards[SG_BLACK] & mask) == stones.bw_bitboards[SG_BLACK]
                    && (board.bw_bitboards[SG_WHITE] & mask) == stones.bw_bitboards[SG_WHITE]) {
                    SgPoint move_copy = move;
                    translate(-dx, -dy); // translate back into original position
                    return move_copy;
                }

        return SG_PASS;
    }

};
