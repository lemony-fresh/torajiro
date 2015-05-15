#pragma once

#include <bitset>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "util.h"

#include "GoBoard.h"
#include "SgPoint.h"
#include "SgBoardColor.h"
#include "SgBWArray.h"

template <std::size_t N> class Bitboard;

template <std::size_t N> using BitboardPtr = std::shared_ptr<Bitboard<N> >;

// N defines the board size (e.g., 9 or 19)
template <std::size_t N>
class Bitboard
{
private:

    SgBWArray<std::bitset<N*N> > bw_bitboards;

    static std::size_t twoDto1D(std::size_t x, std::size_t y) {
        return y * N + x;
    }

public:

    Bitboard() {}

    Bitboard(GoBoard const & other) {
        assert(N == other.Size());
        for (std::size_t y = 1; y <= N; ++y)
            for (std::size_t x = 1; x <= N; ++x) {
                SgPoint p = SgPointUtil::Pt(x, y);
                set(p, other.GetColor(p));
            }
    }

    Bitboard(Bitboard<N> const & other) {
        for (SgBWIterator it; it; ++it)
            bw_bitboards[*it] = other.bw_bitboards[*it];
    }

    /**
     * Short-hand to directly construct a new
     * Bitboard and obtain a shared pointer to it.
     */
    template <typename... Args>
    static BitboardPtr<N> create(Args&&... args) {
      return std::make_shared<Bitboard<N> >(std::forward<Args>(args)...);
    }

    SgBoardColor get(SgPoint const & p) const {
        std::size_t const i = twoDto1D(SgPointUtil::Col(p) - 1, SgPointUtil::Row(p) - 1);
        for (SgBWIterator it; it; ++it)
            if (bw_bitboards[*it][i]) return *it;
        return SG_EMPTY;
    }

    void set(SgPoint const & p, SgBoardColor const & color) {
        std::size_t const i = twoDto1D(SgPointUtil::Col(p) - 1, SgPointUtil::Row(p) - 1);
        if (color == SG_BLACK || color == SG_WHITE)
            for (SgBWIterator it; it; ++it)
                bw_bitboards[*it][i] = (*it == color);
        else if (color == SG_EMPTY)
            for (SgBWIterator it; it; ++it)
                bw_bitboards[*it][i] = false;
        assert(SgIsEmptyBlackWhite(color));
    }

    void invert() {
        std::swap(bw_bitboards[SG_BLACK], bw_bitboards[SG_WHITE]);
    }

    friend std::ostream& operator<<(std::ostream& o, Bitboard const & b) {
        o << "\n  ";
        if (N >= 10)
            o << " ";
        for (std::size_t y = 1; y <= N; ++y)
            o << SgPointUtil::Letter(y) << " ";
        o << "\n";
        for (std::size_t y = 1; y <= N; ++y) {
            o << (y < 10 && N >= 10 ? " " : "") << y << " ";
            for (std::size_t x = 1; x <= N; ++x) {
                SgPoint p = SgPointUtil::Pt(x,y);
                switch (b.get(p)) {
                case SG_WHITE: o << "O "; break;
                case SG_BLACK: o << "X "; break;
                case SG_EMPTY: o << (util::is_star_point<N>(p) ? "+ " : ". "); break;
                default: throw std::invalid_argument("color was neither white, nor black, nor empty");
                }
            }
            o << (N > 9 && y < 10 ? " " : "") << y << "\n";
        }
        o << "  ";
        if (N >= 10)
            o << " ";
        for (std::size_t y = 1; y <= N; ++y)
            o << SgPointUtil::Letter(y) << " ";
        o << std::endl;
        return o;
    }
};
