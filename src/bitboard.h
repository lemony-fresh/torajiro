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
template <std::size_t N> class Pattern;

template <std::size_t N> using BitboardPtr = std::shared_ptr<Bitboard<N> >;

// N defines the board size (e.g., 9 or 19)
template <std::size_t N>
class Bitboard
{

private:

    SgBWArray<std::bitset<N*N> > bw_bitboards;

    void
    swap(SgPoint const & a, SgPoint const & b){
        SgBoardColor temp = this->get(a);
        this->set(a, this->get(b));
        this->set(b, temp);
    }

public:

    friend Pattern<N>;

    Bitboard() {}

    Bitboard(GoBoard const & other) {

        tora_equals(N, other.Size());

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

    bool operator==(Bitboard<N> const & rhs) {
        for (SgBWIterator it; it; ++it)
            if (bw_bitboards[*it] != rhs.bw_bitboards[*it])
                return false;
        return true;
    }
    bool operator!=(Bitboard<N> const & rhs){ return !(this == rhs); }

    /**
     * Short-hand to directly construct a new
     * Bitboard and obtain a shared pointer to it.
     */
    template <typename... Args>
    static BitboardPtr<N> create(Args&&... args) {
        return std::make_shared<Bitboard<N> >(std::forward<Args>(args)...);
    }

    SgBoardColor get(SgPoint const & p) const {
        std::size_t const i = util::twoDto1D<N>(SgPointUtil::Col(p) - 1, SgPointUtil::Row(p) - 1);
        for (SgBWIterator it; it; ++it)
            if (bw_bitboards[*it][i]) return *it;
        return SG_EMPTY;
    }

    void set(SgPoint const & p, SgBoardColor const & color) {
        std::size_t const i = util::twoDto1D<N>(SgPointUtil::Col(p) - 1, SgPointUtil::Row(p) - 1);
        if (color == SG_BLACK || color == SG_WHITE)
            for (SgBWIterator it; it; ++it)
                bw_bitboards[*it][i] = (*it == color);
        else if (color == SG_EMPTY)
            for (SgBWIterator it; it; ++it)
                bw_bitboards[*it][i] = false;

        tora_assert(SgIsEmptyBlackWhite(color));
    }

    void invert() {
        std::swap(bw_bitboards[SG_BLACK], bw_bitboards[SG_WHITE]);
    }

    void flip_horizontally() {
        for(std::size_t y = 1; y <= N; ++y) {
            for(std::size_t x1 = 1; x1 <= N/2; ++x1) {
                std::size_t x2 = N + 1 - x1;
                swap(SgPointUtil::Pt(x1, y), SgPointUtil::Pt(x2, y));
            }
        }
    }

    void flip_vertically() {
        for(std::size_t y1 = 1; y1 <= N/2; ++y1) {
            std::size_t y2 = N + 1 - y1;
            for(std::size_t x = 1; x <= N; ++x)
                swap(SgPointUtil::Pt(x, y1), SgPointUtil::Pt(x, y2));
        }
    }

    void flip_on_secondary_diagonal() {
        for(std::size_t y1 = 1; y1 <= N; ++y1) {
            std::size_t x2 = N + 1 - y1;
            for(std::size_t x1 = 1; x1 < x2; ++x1) {
                std::size_t y2 = N + 1 - x1;
                swap(SgPointUtil::Pt(x1, y1), SgPointUtil::Pt(x2, y2));
            }
        }
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
                case SG_WHITE: o << "W "; break;
                case SG_BLACK: o << "B "; break;
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
