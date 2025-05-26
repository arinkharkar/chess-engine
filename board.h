//
// Created by arin on 5/25/25.
//

#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <ostream>
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
    BLACK = 1 << 7,
    PAWN_BLACK = 1 << 0 | BLACK,
    ROOK_BLACK = 1 << 1 | BLACK,
    KNIGHT_BLACK = 1 << 2 | BLACK,
    BISHOP_BLACK = 1 << 3 | BLACK,
    QUEEN_BLACK = 1 << 4 | BLACK,
    KING_BLACK = 1 << 5 | BLACK,
    PAWN_WHITE= 1 << 0 | WHITE,
    ROOK_WHITE = 1 << 1 | WHITE,
    KNIGHT_WHITE = 1 << 2 | WHITE,
    BISHOP_WHITE = 1 << 3 | WHITE,
    QUEEN_WHITE = 1 << 4 | WHITE,
    KING_WHITE = 1 << 5 | WHITE
};

inline bool is_white(const piece p) {
    return p & WHITE;
}

inline bool is_black(const piece p) {
    return p & BLACK;
}

// if both the pieces are of opposite colors, returns true
// if one of the pieces are NO_PIECE, return false
inline bool are_opposite_colors(const piece p1, const piece p2) {
    if (p1 == NO_PIECE || p2 == NO_PIECE)
        return false;
    if (is_white(p1) && is_white(p2))
        return false;
    if (is_black(p1) && is_black(p2))
        return false;
    return true;
}

static piece get_color(const piece p) {
    if (p & WHITE)
        return WHITE;
    else if (p & BLACK)
        return BLACK;
    return NO_PIECE;
}


static piece remove_color(const piece p) {
    if (is_white(p)) {
        return piece(p & ~WHITE);
        std::cout << "stripped color: " << p << std::endl;
    } else if (is_black(p)) {
        return piece(p & ~BLACK);
        std::cout << "stripped color: " << p << std::endl;
    } else {
        return p;
    }
}

enum ranks {
    RANK_ONE = 1,
    RANK_TWO = 2,
    RANK_THREE = 3,
    RANK_FOUR = 4,
    RANK_FIVE = 5,
    RANK_SIX = 6,
    RANK_SEVEN = 7,
    RANK_EIGHT = 8
};

enum files {
    FILE_A = 1,
    FILE_B = 2,
    FILE_C = 3,
    FILE_D = 4,
    FILE_E = 5,
    FILE_F = 6,
    FILE_G = 7,
    FILE_H = 8
};

enum class turn {
    white = WHITE,
    black = BLACK
};

typedef std::pair<int, int> position;

constexpr const char* FEN_DEFAULT = "8/8/8/8/6R1/8/4k3/8 w - - 0 1";
constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;

class move {
public:
    piece p;
    position start;
    position end;
    bool is_capture;

    move() {
        p = NO_PIECE;
        start = std::make_pair(-1, -1);
        end = std::make_pair(-1, -1);
        is_capture = false;
    }
    move(piece p, position start, position end) {
        this->p = p;
        this->start = start;
        this->end = end;
        this->is_capture = false;
    }

    move(piece p, position start, position end, bool is_capture) {
        this->p = p;
        this->start = start;
        this->end = end;
        this->is_capture = is_capture;
    }
};

class board {
private:
    piece m_board[BOARD_WIDTH+1][BOARD_HEIGHT+1];
    turn m_turn;
    bool m_is_check = false;
    bool m_is_checkmate = false;
    bool m_white_queen_castle = false;
    bool m_white_king_castle = false;
    bool m_black_queen_castle = false;
    bool m_black_king_castle = false;
    int m_halfmove_number = 1;
    int m_fullmove_number = 1;
    // the position, if any, of the current en passant
    position m_en_passant_position = std::make_pair(0, 0);


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

    [[nodiscard]] turn get_turn() const;

    [[nodiscard]] position find_piece(piece p) const;
};



#endif //BOARD_H
