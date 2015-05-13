#pragma once

#include <bitset>
#include <ostream>
#include <stdexcept>
#include <string>

#include "util.h"

#include "GoBoard.h"
#include "SgPoint.h"
#include "SgBoardColor.h"
#include "SgBWArray.h"

// N defines the board size (e.g., 9 or 19)
template <int N>
class Bitboard
{
private:

    SgBWArray<std::bitset<N*N> > bw_bitboards;

    inline int twoDto1D(int x, int y) const {
        return y * N + x;
    }

public:
    Bitboard(){
    }

    Bitboard(const GoBoard& other){
        assert(N == other.Size());
        for(int y = 1; y <= N; ++y)
            for(int x = 1; x <= N; ++x){
                SgPoint p = SgPointUtil::Pt(x, y);
                set(p, other.GetColor(p));
            }
    }

    bool is_occupied(SgPoint const & p) const {
        int i = twoDto1D(SgPointUtil::Col(p) - 1, SgPointUtil::Row(p) - 1);
        for(SgBWIterator it; it; ++it)
            if(bw_bitboards[*it][i]) return true;
        return false;
    }

    SgBoardColor get(SgPoint const & p) const {
        int i = twoDto1D(SgPointUtil::Col(p) - 1, SgPointUtil::Row(p) - 1);
        for(SgBWIterator it; it; ++it)
            if(bw_bitboards[*it][i]) return *it;
        return SG_EMPTY;
    }

    void set(SgPoint const & p, SgBoardColor const & color) {
        int i = twoDto1D(SgPointUtil::Col(p) - 1, SgPointUtil::Row(p) - 1);
        if (color == SG_BLACK || color == SG_WHITE)
            for (SgBWIterator it; it; ++it)
                bw_bitboards[*it][i] = (*it == color);
        else if(color == SG_EMPTY)
            for(SgBWIterator it; it; ++it)
                bw_bitboards[*it][i] = false;
        assert(color == SG_BLACK || color == SG_WHITE || color == SG_EMPTY);
    }

    friend std::ostream& operator<<(std::ostream& o, const Bitboard& b) {
        o << "\n  ";
        if (N >= 10)
            o << " ";
        for (int y = 1; y <= N; ++y)
            o << SgPointUtil::Letter(y) << " ";
        o << "\n";
        for (int y = 1; y <= N; ++y){
            o << (y < 10 && N >= 10 ? " " : "") << y << " ";
            for (int x = 1; x <= N; ++x) {
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
        for(int y = 1; y <= N; ++y)
            o << SgPointUtil::Letter(y) << " ";
        o << std::endl;
        return o;
    }
};
