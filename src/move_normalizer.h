#pragma once

#include "SgBoardColor.h"
#include "SgPoint.h"

template <int N>
void
swap(Bitboard<N> & board, SgPoint const & a, SgPoint const & b){
    SgBoardColor temp = board.get(a);
    board.set(a, board.get(b));
    board.set(b, temp);
}

template <int N>
void
flip_horizontally(std::shared_ptr<std::pair<Bitboard<N>, GoPlayerMove>> move) {
    for(std::size_t y = 1; y <= N; ++y) {
        for(std::size_t x1 = 1; x1 <= N/2; ++x1) {
            std::size_t x2 = N + 1 - x1;
            swap(move->first, SgPointUtil::Pt(x1, y), SgPointUtil::Pt(x2, y));
        }
    }

    SgPoint rotated = SgPointUtil::Rotate(1, move->second.Point(), N);
    move->second.SetPoint(rotated);
}

template <int N>
void
flip_vertically(std::shared_ptr<std::pair<Bitboard<N>, GoPlayerMove>> move) {
    for(std::size_t y1 = 1; y1 <= N/2; ++y1) {
        std::size_t y2 = N + 1 - y1;
        for(std::size_t x = 1; x <= N; ++x)
            swap(move->first, SgPointUtil::Pt(x, y1), SgPointUtil::Pt(x, y2));
    }

    SgPoint rotated = SgPointUtil::Rotate(2, move->second.Point(), N);
    move->second.SetPoint(rotated);
}

template <int N>
void
flip_on_secondary_diagonal(std::shared_ptr<std::pair<Bitboard<N>, GoPlayerMove>> move) {
    for(std::size_t y1 = 1; y1 <= N; ++y1) {
        std::size_t x2 = N + 1 - y1;
        for(std::size_t x1 = 1; x1 < x2; ++x1) {
            std::size_t y2 = N + 1 - x1;
            swap(move->first, SgPointUtil::Pt(x1, y1), SgPointUtil::Pt(x2, y2));
        }
    }

    SgPoint rotated = SgPointUtil::Rotate(7, move->second.Point(), N);
    move->second.SetPoint(rotated);
}

template <int N>
void
normalize_all_moves(std::vector<std::shared_ptr<std::pair<Bitboard<N>, GoPlayerMove>>> & moves) {
    for (std::shared_ptr<std::pair<Bitboard<N>, GoPlayerMove>> move : moves) {
        int const center = N/2 + 1;
        // if in bottom half
        if(SgPointUtil::Row(move->second.Point()) > center)
            flip_vertically(move);
        // if in left half
        if(SgPointUtil::Col(move->second.Point()) < center)
            flip_horizontally(move);
        // if in top left triangle
        if(SgPointUtil::Row(move->second.Point()) < N + 1 - SgPointUtil::Col(move->second.Point()))
            flip_on_secondary_diagonal(move);
    }
}
