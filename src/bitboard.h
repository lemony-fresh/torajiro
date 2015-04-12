#pragma once

#include <bitset>
#include <ostream>
#include <stdexcept>
#include <string>

#include "util.h"

#include "GoBoard.h"
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
        for(int y = 0; y < N; ++y)
            for(int x = 0; x < N; ++x){
                SgPoint p = SgPointUtil::Pt(x+1, y+1);
                set(x, y, other.GetColor(p));
            }
    }

    inline bool is_occupied(int i) const {
        for(SgBWIterator it; it; ++it)
            if(bw_bitboards[*it][i]) return true;
        return false;
    }

    inline bool is_occupied(int x, int y) const {
        return is_occupied(twoDto1D(x, y));
    }

    inline SgBoardColor get(int i) const {
        for(SgBWIterator it; it; ++it)
            if(bw_bitboards[*it][i]) return *it;
        return SG_EMPTY;
    }

    inline SgBoardColor get(int x, int y) const {
        return get(twoDto1D(x, y));
    }

    inline void set(int i, SgBoardColor color){
        if(color == SG_BLACK || color == SG_WHITE)
            bw_bitboards[color][i] = true;
        else if(color == SG_EMPTY)
            for(SgBWIterator it; it; ++it)
                bw_bitboards[*it][i] = false;
        assert(color == SG_BLACK || color == SG_WHITE || color == SG_EMPTY);
    }

    inline void set(int x, int y, SgBoardColor color){
        set(twoDto1D(x, y), color);
    }

    friend std::ostream& operator<<(std::ostream& o, const Bitboard& b) {
        o << "\n  ";
        if (N >= 10)
            o << " ";
        for (int y = 1; y <= N; ++y)
            o << SgPointUtil::Letter(y) << " ";
        o << "\n";
        for (int y = N-1; y >= 0; --y){
            o << (y+1 < 10 && N >= 10 ? " " : "") << y + 1 << " ";
            for (int x = 0; x < N; ++x)
                switch(b.get(x, y)){
                case SG_WHITE: o << "O "; break;
                case SG_BLACK: o << "X "; break;
                case SG_EMPTY: o << (util::is_star_point<N>(x,y) ? "+ " : ". "); break;
                default: throw std::invalid_argument("color was neither white, nor black, nor empty");
                }
            o << (N > 9 && y+1 < 10 ? " " : "") << y + 1 << "\n";
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
