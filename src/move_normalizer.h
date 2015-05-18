#pragma once

#include "SgBoardColor.h"
#include "SgPoint.h"

template <std::size_t N>
void
flip_horizontally(std::pair<BitboardPtr<N>, GoPlayerMove> & move) {
    move.first->flip_horizontally();
    SgPoint rotated = SgPointUtil::Rotate(1, move.second.Point(), N);
    move.second.SetPoint(rotated);
}

template <std::size_t N>
void
flip_vertically(std::pair<BitboardPtr<N>, GoPlayerMove> & move) {
    move.first->flip_vertically();
    SgPoint rotated = SgPointUtil::Rotate(2, move.second.Point(), N);
    move.second.SetPoint(rotated);
}

template <std::size_t N>
void
flip_on_secondary_diagonal(std::pair<BitboardPtr<N>, GoPlayerMove> & move) {
    move.first->flip_on_secondary_diagonal();
    SgPoint rotated = SgPointUtil::Rotate(7, move.second.Point(), N);
    move.second.SetPoint(rotated);
}

/**
 * Rotates all boards and moves such that the move is in a
 * normalized position (where the opening move is usually played)
 */
template <std::size_t N>
void
normalize_all_moves(std::vector<std::pair<BitboardPtr<N>, GoPlayerMove>> & training_set) {
    for (std::pair<BitboardPtr<N>, GoPlayerMove> & training_ex : training_set) {
        // invert color if white is to move
        if (training_ex.second.Color() == SG_WHITE) {
            training_ex.first->invert();
            training_ex.second = GoPlayerMove(SG_BLACK, training_ex.second.Point());
        }

        int const center = N/2 + 1;
        // if in bottom half
        if(SgPointUtil::Row(training_ex.second.Point()) > center)
            flip_vertically(training_ex);
        // if in left half
        if(SgPointUtil::Col(training_ex.second.Point()) < center)
            flip_horizontally(training_ex);
        // if in top left triangle
        if(SgPointUtil::Row(training_ex.second.Point()) <
            (int)N + 1 - SgPointUtil::Col(training_ex.second.Point()))
            flip_on_secondary_diagonal(training_ex);
    }
}
