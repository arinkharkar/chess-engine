//
// Created by arin on 5/25/25.
//

#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <ostream>
#include <string>

constexpr const char* FEN_DEFAULT = "8/8/8/br6/8/8/8/R3K3 w Q - 0 1";

enum Piece {
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


inline std::string to_string(Piece piece) {
    std::string return_string;

    if (piece == NO_PIECE) return "NO_PIECE";

    if (piece & WHITE) return_string = "WHITE";
    else if (piece & BLACK) return_string = "BLACK";

    switch (piece & 0x3F) { // Mask out color bits
        case PAWN:   return_string += "_PAWN"; break;
        case ROOK:   return_string += "_ROOK"; break;
        case KNIGHT: return_string += "_KNIGHT"; break;
        case BISHOP: return_string += "_BISHOP"; break;
        case QUEEN:  return_string += "_QUEEN"; break;
        case KING:   return_string += "_KING"; break;
        default:     return "UNKNOWN_PIECE";
    }
    return return_string;
}


inline bool is_white(const Piece piece) {
    return piece & WHITE;
}

inline bool is_black(const Piece piece) {
    return piece & BLACK;
}

// if both the pieces are of opposite colors, returns true
// if one of the pieces are NO_PIECE, return false
inline bool are_opposite_colors(const Piece piece1, const Piece piece2) {
    if (piece1 == NO_PIECE || piece2 == NO_PIECE)
        return false;
    if (is_white(piece1) && is_white(piece2))
        return false;
    if (is_black(piece1) && is_black(piece2))
        return false;
    return true;
}

static Piece get_color(const Piece p) {
    if (p & WHITE)
        return WHITE;
    else if (p & BLACK)
        return BLACK;
    return NO_PIECE;
}


static Piece remove_color(const Piece piece) {
    if (is_white(piece)) {
        return Piece(piece & ~WHITE);
        std::cout << "stripped color: " << piece << std::endl;
    } else if (is_black(piece)) {
        return Piece(piece & ~BLACK);
        std::cout << "stripped color: " << piece << std::endl;
    } else {
        return piece;
    }
}

enum Ranks {
    RANK_ONE = 1,
    RANK_TWO = 2,
    RANK_THREE = 3,
    RANK_FOUR = 4,
    RANK_FIVE = 5,
    RANK_SIX = 6,
    RANK_SEVEN = 7,
    RANK_EIGHT = 8
};

enum Files {
    FILE_A = 1,
    FILE_B = 2,
    FILE_C = 3,
    FILE_D = 4,
    FILE_E = 5,
    FILE_F = 6,
    FILE_G = 7,
    FILE_H = 8
};

enum class Turn {
    white = WHITE,
    black = BLACK
};

typedef std::pair<int, int> Position;

constexpr int BOARD_WIDTH = 8;
constexpr int BOARD_HEIGHT = 8;



class Board {
private:
    Piece m_board[BOARD_WIDTH+1][BOARD_HEIGHT+1];
    Turn m_turn;
    bool m_is_check = false;
    bool m_is_checkmate = false;

    int m_halfmove_number = 1;
    int m_fullmove_number = 1;
    // the position, if any, of the current en passant
    Position m_en_passant_position = std::make_pair(0, 0);


public:
    bool m_white_queen_castle = false;
    bool m_white_king_castle = false;
    bool m_black_queen_castle = false;
    bool m_black_king_castle = false;
    // Initialize a starting board
    Board();
    // Initialize a board with a certain FEN
    Board(const std::string& fen);
    // Reset the current board
    void reset_board();

    void print();

    static bool in_bounds(Position pos);

    static bool in_bounds(int x, int y);

    // Place a piece on the board
    void place_piece(Piece placed_piece, Position pos);

    void place_piece(Piece placed_piece, int x, int y);

    [[nodiscard]] Piece get_piece(int x, int y) const;
    // Get a piece at a positin
    [[nodiscard]] Piece get_piece(Position pos) const;

    [[nodiscard]] Turn get_turn() const;

    void set_turn(Turn t) {
        m_turn = t;
    }

    void next_turn() {
        if (m_turn == Turn::white)
            m_turn = Turn::black;
        else if (m_turn == Turn::black)
            m_turn = Turn::white;
    }

    [[nodiscard]] Position find_piece(Piece p) const;
};

class move {
public:
    Piece p;
    Position start;
    Position end;
    bool is_capture;

    move() {
        p = NO_PIECE;
        start = std::make_pair(-1, -1);
        end = std::make_pair(-1, -1);
        is_capture = false;
    }
    move(Board* b, Position start, Position end) {
        this->p = b->get_piece(start);
        this->start = start;
        this->end = end;
        this->is_capture = false;
    }

    move(Piece p, Position start, Position end) {
        this->p = p;
        this->start = start;
        this->end = end;
        this->is_capture = false;
    }

    move(Piece p, Position start, Position end, bool is_capture) {
        this->p = p;
        this->start = start;
        this->end = end;
        this->is_capture = is_capture;
    }
};

#endif //BOARD_H
