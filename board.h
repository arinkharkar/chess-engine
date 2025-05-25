//
// Created by arin on 5/25/25.
//

#ifndef BOARD_H
#define BOARD_H
#include <string>

enum piece {
    NO_PIECE = 0,
    PAWN = 1 << 0,
    ROOK = 1 << 1,
    KNIGHT = 1 << 2,
    BISHOP = 1 << 3,
    QUEEN = 1 << 4,
    KING = 1 << 5,
    WHITE = 1 << 6,
    BLACK = 1 << 7
};

enum class turn {
    white,
    black
};

typedef std::pair<int, int> position;

constexpr std::string FEN_DEFAULT = "r";
constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;


class board {
private:
    piece m_board[BOARD_WIDTH][BOARD_HEIGHT];
    turn m_turn;
    bool is_check;
    bool is_checkmate;



public:
    // Initialize a starting board
    board();
    // Initialize a board with a certain FEN
    board(const std::string& fen);
    // Reset the current board
    void reset_board();
    // Place a piece on the board
    void place_piece(piece placed_piece, position pos);
    // Get a piece at a positin
    [[nodiscard]] piece get_piece(position pos) const;
};



#endif //BOARD_H
