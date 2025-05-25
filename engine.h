//
// Created by arin on 5/25/25.
//

#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include "board.h"

constexpr int DEFAULT_MAX_DEPTH = 1;

class engine {
private:
    int m_max_depth;
    board* m_board;
public:
    engine();
    ~engine();


    std::vector<move> get_legal_moves();

    std::vector<move> get_legal_piece_moves(piece p, position pos);

    void print_board();

    void generate_pawn_moves(position& pos, std::vector<move>* moves);
    void generate_knight_moves(position& pos, std::vector<move>* moves);
    void generate_bishop_moves(position& pos, std::vector<move>* moves);
    void generate_rook_moves(position& pos, std::vector<move>* moves);
    void generate_queen_moves(position& pos, std::vector<move>* moves);
    void generate_king_moves(position& pos, std::vector<move>* moves);


    bool is_legal_move(move m);
};



#endif //ENGINE_H
