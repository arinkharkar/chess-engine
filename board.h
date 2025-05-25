//
// Created by arin on 5/25/25.
//

#ifndef BOARD_H
#define BOARD_H
#include <string>

enum piece {
    NO_PIECE = 0,
    PAWN_BLACK = 1 << 0,
    ROOK_BLACK = 1 << 1,
    KNIGHT_BLACK = 1 << 2,
    BISHOP_BLACK = 1 << 3,
    QUEEN_BLACK = 1 << 4,
    KING_BLACK = 1 << 5,
    PAWN = 1 << 0,
    ROOK = 1 << 1,
    KNIGHT = 1 << 2,
    BISHOP = 1 << 3,
    QUEEN = 1 << 4,
    KING = 1 << 5,
    WHITE = 1 << 6,
    BLACK = 0,
    PAWN_WHITE= 1 << 0 | WHITE,
    ROOK_WHITE = 1 << 1 | WHITE,
    KNIGHT_WHITE = 1 << 2 | WHITE,
    BISHOP_WHITE = 1 << 3 | WHITE,
    QUEEN_WHITE = 1 << 4 | WHITE,
    KING_WHITE = 1 << 5 | WHITE
};

enum class turn {
    white,
    black
};

typedef std::pair<int, int> position;

constexpr const char* FEN_DEFAULT = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;

class move {
private:
    piece m_p;
    position m_start;
    position m_end;

public:
    move() {
        m_p = NO_PIECE;
        m_start.first = -1;
        m_start.second = -1;
        m_end.first = -1;
        m_end.second = -1;
    }

    move(piece p, position start, position end) {
        m_p = p;
        m_start = start;
        m_end = end;
    }
};

class board {
private:
    piece m_board[BOARD_WIDTH][BOARD_HEIGHT];
    turn m_turn;
    bool is_check;
    bool is_checkmate;
    bool white_can_castle;
    bool black_can_castle;



public:
    // Initialize a starting board
    board();
    // Initialize a board with a certain FEN
    board(const std::string& fen);
    // Reset the current board
    void reset_board();

    void print();

    // Place a piece on the board
    void place_piece(piece placed_piece, position pos);

    void place_piece(piece placed_piece, int x, int y);

    [[nodiscard]] piece get_piece(int x, int y) const;
    // Get a piece at a positin
    [[nodiscard]] piece get_piece(position pos) const;
};



#endif //BOARD_H
