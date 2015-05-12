#pragma once

#include "SgBoardColor.h"
#include "SgPoint.h"

template <int N>
void
swap(std::shared_ptr<Bitboard<N>> board, int x1, int y1, int x2, int y2){
    SgBoardColor temp = board->get(x1, y1);
    board->set(x1, y1, board->get(x2, y2));
    board->set(x2, y2, temp);
}

template <int N>
void
flip_horizontally(std::pair<std::shared_ptr<Bitboard<N>>, GoPlayerMove> & move) {
    for(std::size_t y = 0; y < N; ++y) {
        for(std::size_t x1 = 0; x1 < N/2; ++x1) {
            std::size_t x2 = N - 1 - x1;
            swap(move.first, x1, y, x2, y);
            SgPoint rotated = SgPointUtil::Rotate(1, move.second.Point(), N);
            move.second.SetPoint(rotated);
        }
    }
}

template <int N>
void
flip_vertically(std::pair<std::shared_ptr<Bitboard<N>>, GoPlayerMove> & move) {
    for(std::size_t y1 = 0; y1 < N/2; ++y1) {
        std::size_t y2 = N - 1 - y1;
        for(std::size_t x = 0; x < N; ++x) {
            swap(move.first, x, y1, x, y2);
            SgPoint rotated = SgPointUtil::Rotate(2, move.second.Point(), N);
            move.second.SetPoint(rotated);
        }
    }
}

template <int N>
void
flip_on_secondary_diagonal(std::pair<std::shared_ptr<Bitboard<N>>, GoPlayerMove> & move) {
    for(std::size_t y1 = 0; y1 < N; ++y1) {
        std::size_t x2 = N - 1 - y1;
        for(std::size_t x1 = 0; x1 < x2; ++x1) {
            std::size_t y2 = N - 1 - x1;
            swap(move.first, x1, y1, x2, y2);
            SgPoint rotated = SgPointUtil::Rotate(7, move.second.Point(), N);
            move.second.SetPoint(rotated);
        }
    }
}

template <int N>
void
normalize_all_moves(std::vector<std::pair<std::shared_ptr<Bitboard<N>>, GoPlayerMove>> & moves) {
    for (std::pair<std::shared_ptr<Bitboard<N>>, GoPlayerMove> & move : moves) {
        int const center = N/2 + 1;
        // if in bottom half
        if(SgPointUtil::Row(move.second.Point()) > center)
            flip_vertically(move);
        // if in left half
        if(SgPointUtil::Col(move.second.Point()) < center)
            flip_horizontally(move);
        // if in top left triangle
        if(SgPointUtil::Row(move.second.Point()) <= N - SgPointUtil::Col(move.second.Point()))
            flip_on_secondary_diagonal(move);
    }

    // TODO: kein x und y mehr benutzen, alles auf SgPoint umstellen und die Umrechnung (-1) in Bitboard machen

    // TODO: test cases schreiben: bitboard per hand bestuecken, flippen (oder normalisieren lassen), abfragen.

    // TODO: OMP fuer die Normalisierung ausprobieren
}
