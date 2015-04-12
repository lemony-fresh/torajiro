#pragma once

#include <vector>
#include <string>
#include <utility>

#include "GoGame.h"
#include "GoBoard.h"
#include "GoPlayerMove.h"

#include "bitboard.h"

/**
 * @brief read_games reads all go games below the given path
 * @param path if path is a file, only this file is read, otherwise all sgf files below this path are read.
 * @return
 */
std::vector<std::shared_ptr<GoGame> > read_games(std::string const& path);

/**
 * @brief extract_moves given some games it extracts all positions and the moves played in those positions
 * @param games
 * @return
 */
template <int N>
std::vector<std::pair<std::shared_ptr<Bitboard<N> >, GoPlayerMove> >
extract_moves(std::vector<std::shared_ptr<GoGame> > const& games) {

    std::vector<std::pair<std::shared_ptr<Bitboard<N> >,GoPlayerMove> > moves;

    for(auto const& game : games) {
        while(true){
            SgMove move = game->CurrentMove();
            if(SgIsSpecialMove(move))
                break;
            GoPlayerMove player_move(game->Board().ToPlay(), game->CurrentMove());
            auto board = std::make_shared<Bitboard<N> >(game->Board());
            moves.push_back(std::make_pair(board, player_move));
            if(!game->CanGoInDirection(SgNode::Direction::MAIN_BRANCH))
                break;
            game->GoInDirection(SgNode::Direction::MAIN_BRANCH);
        }
    }

    return moves;
}
