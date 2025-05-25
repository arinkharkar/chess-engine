//
// Created by arin on 5/25/25.
//

#include "board.h"

board::board() : board(FEN_DEFAULT) {}

board::board(const std::string &fen) {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            m_board[x][y] = NO_PIECE;
        }
    }
    m_turn = turn::white;
    is_check = false;
    is_checkmate = false;
}


void board::reset_board() {
    board(FEN_DEFAULT);
}

piece board::get_piece(position pos) const {
    return m_board[pos.first][pos.second];
}

void board::place_piece(piece placed_piece, position pos) {
    m_board[pos.first][pos.second] = placed_piece;
}

