//
// Created by arin on 5/25/25.
//

#include "engine.h"

engine::engine() {
    m_max_depth = DEFAULT_MAX_DEPTH;
    m_board = new board();
    m_board->place_piece(BISHOP, 4, 4);
}

std::vector<move> engine::get_legal_moves() {
    std::vector<move> legal_moves;
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            get_legal_piece_moves(m_board->get_piece(x,y), position(x,y));
        }
    }
}

void engine::print_board() {
    m_board->print();
}


std::vector<move> engine::get_legal_piece_moves(piece p, position pos) {
    std::vector<move> legal_moves;
    switch (p) {
        case piece::PAWN:
            generate_pawn_moves(pos, &legal_moves);
            break;
        case piece::KNIGHT:
     //       generate_knight_moves(pos, &legal_moves);
            break;
        case piece::BISHOP:
            generate_bishop_moves(pos, &legal_moves);
            break;
        case piece::ROOK:
       //     generate_rook_moves(pos, &legal_moves);
            break;
        case piece::QUEEN:
      //      generate_queen_moves(pos, &legal_moves);
            break;
        case piece::KING:
       //     generate_king_moves(pos, &legal_moves);
            break;
    }
}

void engine::generate_pawn_moves(position& pos, std::vector<move>* legal_moves) {
    // pawns can move forward, 2x forward, and once diagonally (for caputures)
    position new_pos = pos;
    new_pos.second = new_pos.second + 1;
    legal_moves->push_back(move(PAWN, pos, new_pos));
    new_pos.second = new_pos.second + 1;
    legal_moves->push_back(move(PAWN, pos, new_pos));
}

void engine::generate_bishop_moves(position& pos, std::vector<move>* legal_moves) {
    position new_pos = pos;
    // go down diagonally
    while (new_pos.first != -1 || new_pos.second != -1) {
        new_pos.first--;
        new_pos.second--;
        legal_moves->push_back(move(BISHOP, pos, new_pos));
    }

    new_pos = pos;
    // go up diagonally
    while (new_pos.first != BOARD_WIDTH+1 || new_pos.second != BOARD_HEIGHT+1) {
        new_pos.first++;
        new_pos.second++;
        legal_moves->push_back(move(BISHOP, pos, new_pos));
    }
}


bool engine::is_legal_move(move m) {

}

engine::~engine() {
    delete m_board;
}


