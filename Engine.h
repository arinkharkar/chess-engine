//
// Created by arin on 5/25/25.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "Board.h"

constexpr int DEFAULT_MAX_DEPTH = 1;

class Engine {
private:
    int m_max_depth;
    Board* m_board;

    // gets all legal moves not including checks
    std::vector<Move> get_legal_moves_not_check();

public:
    Engine();
    ~Engine();


    std::vector<Move> get_legal_moves();

    std::vector<Move> get_legal_piece_moves(Piece piece, Position pos);

    void print_board() const;

    void generate_pawn_moves(Position& pos, std::vector<Move>* moves, Piece p) const;
    void generate_knight_moves(Position& pos, std::vector<Move>* moves, Piece p);
    void generate_bishop_moves(Position& pos, std::vector<Move>* moves, Piece piece) const;
    void generate_rook_moves(Position& pos, std::vector<Move>* moves, Piece piece) const;
    void generate_queen_moves(Position& pos, std::vector<Move>* moves, Piece p) const;
    void generate_king_moves(Position& pos, std::vector<Move>* moves, Piece p);

    bool results_in_check(Move& move);

    [[nodiscard]] Board* get_board() const {
        return m_board;
    }
    // returns true if the move was valid, false if not
    bool make_move(Move& move) const;
};



#endif //ENGINE_H
