#pragma once

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
        // TODO copy board
        if (to_move == SG_WHITE)
            // TODO invert the board
            ;

        // TODO
        return SG_PASS;
    }

};
