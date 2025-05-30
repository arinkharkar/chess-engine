//
// Created by arin on 5/25/25.
//

#include "Engine.h"

#include <iostream>

Engine::Engine() {
    m_max_depth = DEFAULT_MAX_DEPTH;
    m_board = new Board();
}

std::vector<Move> Engine::get_legal_moves_not_check() {
    std::vector<Move> legal_moves;
    for (int x = 1; x < BOARD_WIDTH+1; x++) {
        for (int y = 1; y < BOARD_HEIGHT+1; y++) {
            std::vector<Move> piece_legal_moves = get_legal_piece_moves(m_board->get_piece(x,y), Position(x,y));
            if (!piece_legal_moves.empty())
                legal_moves.insert(legal_moves.end(), piece_legal_moves.begin(), piece_legal_moves.end());
        }
    }
    return legal_moves;
}


std::vector<Move> Engine::get_legal_moves() {
    std::vector<Move> starting_legal_moves = get_legal_moves_not_check();
    std::vector<Move> ending_legal_moves;
    for (Move& m : starting_legal_moves) {
        if (!results_in_check(m)) ending_legal_moves.push_back(m);
    }
    return ending_legal_moves;

}


void Engine::print_board() const {
    m_board->print();
}


std::vector<Move> Engine::get_legal_piece_moves(Piece piece, Position pos) {

    std::vector<Move> legal_moves;
    if (piece == NO_PIECE)
        return legal_moves;
    switch (remove_color(piece)) {
        case Piece::PAWN:
            generate_pawn_moves(pos, &legal_moves, piece);
            break;
        case Piece::KNIGHT:
            generate_knight_moves(pos, &legal_moves, piece);
            break;
        case Piece::BISHOP:
            generate_bishop_moves(pos, &legal_moves, piece);
            break;
        case Piece::ROOK:
            generate_rook_moves(pos, &legal_moves, piece);
            break;
        case Piece::QUEEN:
            generate_queen_moves(pos, &legal_moves, piece);
            break;
        case Piece::KING:
            generate_king_moves(pos, &legal_moves, piece);
            break;
        default:
            std::cerr << "Invalid piece: " << to_string(piece) << std::endl;
    }
    return legal_moves;
}

void Engine::generate_pawn_moves(Position& pos, std::vector<Move>* legal_moves, const Piece p) const {
    // if the pawn piece is white and the turn is white
    int forward;
    if (m_board->get_turn() == Turn::white && is_white(p)) {
        forward = 1;
    } else if (m_board->get_turn() == Turn::black && is_black(p)) {
        forward = -1;
    } else {
        // the piece color and the active turn are different, no need to calculate the position
        return;
    }

    // pawns can move forward, 2x forward, and once diagonally (for caputures)

    // first, check if the next position has a piece, if it doesn't add that to the list of legal moves
    Position new_pos = pos;
    new_pos.second = new_pos.second + forward;
    if (m_board->get_piece(new_pos) == NO_PIECE) {
        legal_moves->push_back(Move(p, pos, new_pos));
    }

    // now check for captures
    // first, check the diagonal + 1
    new_pos.first = pos.first + 1;
    if (are_opposite_colors(m_board->get_piece(new_pos), p) && new_pos.first != BOARD_WIDTH + 1) {
        legal_moves->push_back(Move(p, pos, new_pos, true));
    }
    // now check the diagonal - 1
    new_pos.first = pos.first - 1;
    if (are_opposite_colors(m_board->get_piece(new_pos), p) && new_pos.first != 0) {
        legal_moves->push_back(Move(p, pos, new_pos, true));
    }
    // reset the new_pos
    new_pos.first = pos.first;
    // if the pawn is on the 2nd rank, we can move it twice (provided there isnt a piece on that square)
    new_pos.second = new_pos.second + forward;
    if (m_board->get_piece(new_pos) == NO_PIECE && pos.second == 2) {
        legal_moves->push_back(Move(p, pos, new_pos));
    }

    // handling piece promotion
    if (legal_moves->back().end.second == 8 || legal_moves->back().end.second == 1) {
        Move move = legal_moves->back();
        legal_moves->pop_back();
        // if the current color is white
        if (forward == 1) {
            move.piece = ROOK_WHITE;
            legal_moves->push_back(move);
            move.piece = KNIGHT_WHITE;
            legal_moves->push_back(move);
            move.piece = BISHOP_WHITE;
            legal_moves->push_back(move);
            move.piece = QUEEN_WHITE;
            legal_moves->push_back(move);
        } else {
            move.piece = ROOK_BLACK;
            legal_moves->push_back(move);
            move.piece = KNIGHT_BLACK;
            legal_moves->push_back(move);
            move.piece = BISHOP_BLACK;
            legal_moves->push_back(move);
            move.piece = QUEEN_BLACK;
            legal_moves->push_back(move);
        }
    }
}

void Engine::generate_queen_moves(Position &pos, std::vector<Move> *legal_moves, const Piece p) const {
    // a queen is just a bishop and a rook combined
    generate_bishop_moves(pos, legal_moves, p);
    generate_rook_moves(pos, legal_moves, p);
}


void Engine::generate_bishop_moves(Position& pos, std::vector<Move>* legal_moves, const Piece piece) const {
    // if the piece is not the correct color, return
    if (m_board->get_turn() != Turn(get_color(piece)))
        return;
    Position new_pos = pos;

    int directions[4][2] = {
        {-1, 1},
        {1, -1},
        {-1, -1},
        {1, 1}
    };
    constexpr int num_loops = std::size(directions);

    for (int i = 0; i < num_loops; i++) {
        int x_dir = directions[i][0];
        int y_dir = directions[i][1];

        new_pos = pos;
        while (true) {
            new_pos.first += x_dir;
            new_pos.second += y_dir;

            if (!Board::in_bounds(new_pos))
                break;
            // if the position already has a piece on it, we want to end this loop after adding the capture piece

            if (are_opposite_colors(m_board->get_piece(new_pos), piece)) {
                legal_moves->push_back(Move(piece, pos, new_pos, true));
                break;
            } else if (m_board->get_piece(new_pos) != NO_PIECE) {
                break;
            }
            legal_moves->push_back(Move(piece, pos, new_pos));

        }
    }
}

void Engine::generate_rook_moves(Position& pos, std::vector<Move>* legal_moves, const Piece piece) const {
    // if the piece is not the correct color, return
    if (m_board->get_turn() != Turn(get_color(piece)))
        return;
    Position new_pos = pos;

    int directions[4][2] = {
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    };
    constexpr int num_loops = std::size(directions);

    for (int i = 0; i < num_loops; i++) {
        const int x_dir = directions[i][0];
        const int y_dir = directions[i][1];

        new_pos = pos;
        while (true) {
            new_pos.first += x_dir;
            new_pos.second += y_dir;

            if (!Board::in_bounds(new_pos))
                break;
            // if the position already has a piece on it, we want to end this loop after adding the capture piece

            if (are_opposite_colors(m_board->get_piece(new_pos), piece)) {
                legal_moves->push_back(Move(piece, pos, new_pos, true));
                break;
            } else if (m_board->get_piece(new_pos) != NO_PIECE) {
                break;
            }
            legal_moves->push_back(Move(piece, pos, new_pos));

        }
    }
}


bool Engine::results_in_check(Move& move) {
    Piece end_piece = m_board->get_piece(move.end);
    // make the
    make_move(move);
    m_board->next_turn();
    const std::vector<Move> moves = get_legal_moves_not_check();

    m_board->next_turn();
    const Position king_location = m_board->find_piece(Piece(KING | int(m_board->get_turn())));


    // check if this is a castle, the inverse move must be special
    if (move.is_castle()) {
        if (move.end == std::make_pair(1,1)) {
            Move king_move_inverse, rook_move_inverse;
            king_move_inverse.piece = KING_WHITE;
            king_move_inverse.start = std::make_pair(3, 1);
            king_move_inverse.end = std::make_pair(5, 1);

            rook_move_inverse.piece = ROOK_WHITE;
            rook_move_inverse.start = std::make_pair(4, 1);
            rook_move_inverse.end = std::make_pair(1, 1);
            make_move(king_move_inverse);
            make_move(rook_move_inverse);
        } else if (move.end == std::make_pair(8,1)) {
            Move king_move_inverse, rook_move_inverse;
            king_move_inverse.piece = KING_WHITE;
            king_move_inverse.start = std::make_pair(7, 1);
            king_move_inverse.end = std::make_pair(5, 1);

            rook_move_inverse.piece = ROOK_WHITE;
            rook_move_inverse.start = std::make_pair(6, 1);
            rook_move_inverse.end = std::make_pair(8, 1);
            make_move(king_move_inverse);
            make_move(rook_move_inverse);
        } else if (move.end == std::make_pair(1,8)) {
            Move king_move_inverse, rook_move_inverse;
            king_move_inverse.piece = KING_BLACK;
            king_move_inverse.start = std::make_pair(3, 8);
            king_move_inverse.end = std::make_pair(5, 8);

            rook_move_inverse.piece = ROOK_BLACK;
            rook_move_inverse.start = std::make_pair(4, 8);
            rook_move_inverse.end = std::make_pair(1, 8);
            make_move(king_move_inverse);
            make_move(rook_move_inverse);
        } else if (move.end == std::make_pair(8,8)) {
            Move king_move_inverse, rook_move_inverse;
            king_move_inverse.piece = KING_BLACK;
            king_move_inverse.start = std::make_pair(7, 8);
            king_move_inverse.end = std::make_pair(5, 8);

            rook_move_inverse.piece = ROOK_BLACK;
            rook_move_inverse.start = std::make_pair(6, 8);
            rook_move_inverse.end = std::make_pair(8, 8);
            make_move(king_move_inverse);
            make_move(rook_move_inverse);
        }
    }

    Move inverse_move = Move(m_board, move.end, move.start);
    make_move(inverse_move);
    m_board->place_piece(end_piece, move.end);


    for (int i = 0; i < moves.size(); i++) {
        if (moves[i].end == king_location) {
            return true;
        }
    }

    return false;
}



bool Engine::make_move(Move& move) const {
    // make sure the move is valid
    if (move.start.first < 1 || move.start.first > BOARD_WIDTH)
        return false;
    if (move.start.second < 1 || move.start.second > BOARD_HEIGHT)
        return false;
    if (move.end.first < 1 || move.end.first > BOARD_WIDTH)
        return false;
    if (move.end.second < 1 || move.end.second > BOARD_HEIGHT)
        return false;
    // check if this is a castle, handle this differently if it is
    if (move.is_castle()) {
        // checking white queenside castle
        if (move.end == std::make_pair(1, 1) && m_board->get_piece(2, 1) == NO_PIECE && m_board->get_piece(3, 1) == NO_PIECE && m_board->get_piece(4, 1) == NO_PIECE) {
            if (!m_board->m_white_queen_castle)
                return false;

            m_board->place_piece(NO_PIECE, move.start);
            m_board->place_piece(NO_PIECE, move.end);
            m_board->place_piece(KING_WHITE, std::make_pair(3, 1));
            m_board->place_piece(ROOK_WHITE, std::make_pair(4, 1));

            m_board->m_white_queen_castle = false;
            m_board->m_white_king_castle = false;
            return true;
        } else if (move.end == std::make_pair(8, 1) && m_board->get_piece(6, 1) == NO_PIECE && m_board->get_piece(7, 1)) {
            if (!m_board->m_white_king_castle)
                return false;
            m_board->place_piece(NO_PIECE, move.start);
            m_board->place_piece(NO_PIECE, move.end);
            m_board->place_piece(KING_WHITE, std::make_pair(7, 1));
            m_board->place_piece(ROOK_WHITE, std::make_pair(6, 1));
            m_board->m_white_queen_castle = false;
            m_board->m_white_king_castle = false;
            return true;
        } else if (move.end == std::make_pair(1, 8) && m_board->get_piece(2, 8) == NO_PIECE && m_board->get_piece(3, 8) == NO_PIECE && m_board->get_piece(4, 8) == NO_PIECE) {
            if (!m_board->m_black_queen_castle)
                return false;
            m_board->place_piece(NO_PIECE, move.start);
            m_board->place_piece(NO_PIECE, move.end);
            m_board->place_piece(KING_WHITE, std::make_pair(3, 8));
            m_board->place_piece(ROOK_WHITE, std::make_pair(4, 8));

            m_board->m_black_queen_castle = false;
            m_board->m_black_king_castle = false;
            return true;
        } else if (move.end == std::make_pair(8, 1) && m_board->get_piece(6, 1) == NO_PIECE && m_board->get_piece(7, 1)) {
            if (!m_board->m_black_king_castle)
                return false;
            m_board->place_piece(NO_PIECE, move.start);
            m_board->place_piece(NO_PIECE, move.end);
            m_board->place_piece(KING_WHITE, std::make_pair(7, 1));
            m_board->place_piece(ROOK_WHITE, std::make_pair(6, 1));

            m_board->m_black_queen_castle = false;
            m_board->m_black_king_castle = false;
            return true;
        }
    }
    m_board->place_piece(NO_PIECE, move.start);
    m_board->place_piece(move.piece, move.end);
    return true;
}

void Engine::generate_king_moves(Position &pos, std::vector<Move> *legal_moves, Piece p) {
    // if the piece is not the correct color, return
    if (m_board->get_turn() != Turn(get_color(p)))
        return;
    Position new_pos = pos;

    int directions[8][2] = {
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0},
        {-1, 1},
        {1, -1},
        {-1, -1},
        {1, 1}
    };
    constexpr int num_loops = std::size(directions);

    for (int i = 0; i < num_loops; i++) {
        new_pos = pos;
        new_pos.first += directions[i][0];
        new_pos.second += directions[i][1];
        // if the position isnt in the range
        if (new_pos.first < 1 || new_pos.first > BOARD_WIDTH || new_pos.second > BOARD_HEIGHT || new_pos.second < 1) continue;
        if (are_opposite_colors(m_board->get_piece(new_pos), p)) {
            legal_moves->push_back(Move(p, pos, new_pos, true));
        } else if (m_board->get_piece(new_pos) == NO_PIECE) {
            legal_moves->push_back(Move(p, pos, new_pos));
        }

    }

    if (m_board->get_turn() == Turn::white) {
        if (m_board->m_white_king_castle) {
            new_pos.first = 8;
            new_pos.second = 1;
            // check if the squares between the king and the rook are empty
            if (m_board->get_piece(6, 1) == NO_PIECE && m_board->get_piece(7, 1) == NO_PIECE)
                legal_moves->push_back(Move(p, pos, new_pos));

        }
        if (m_board->m_white_queen_castle) {
            new_pos.first = 1;
            new_pos.second = 1;
            if (m_board->get_piece(4, 1) == NO_PIECE && m_board->get_piece(3, 1) == NO_PIECE && m_board->get_piece(2, 1) == NO_PIECE)
                legal_moves->push_back(Move(p, pos, new_pos));

        }
    } else if (m_board->get_turn() == Turn::black) {
        if (m_board->m_black_king_castle) {
            new_pos.first = 8;
            new_pos.second = 8;
            // check if the squares between the king and the rook are empty
            if (m_board->get_piece(6, 8) == NO_PIECE && m_board->get_piece(7, 8) == NO_PIECE)
                legal_moves->push_back(Move(p, pos, new_pos));
        }
        if (m_board->m_black_queen_castle) {
            new_pos.first = 1;
            new_pos.second = 8;
            if (m_board->get_piece(4, 8) == NO_PIECE && m_board->get_piece(3, 8) == NO_PIECE && m_board->get_piece(2, 8) == NO_PIECE)
                legal_moves->push_back(Move(p, pos, new_pos));

        }
    }
}


void Engine::generate_knight_moves(Position &pos, std::vector<Move> *legal_moves, Piece p) {
    // if the piece is not the correct color, return
    if (m_board->get_turn() != Turn(get_color(p)))
        return;

    int directions[8][2] = {
        {1, 2},
        {2, 1},
        {2, -1},
        {1, -2},
        {-1, -2},
        {-2, -1},
        {-2, 1},
        {-1, 2}
    };
    constexpr int num_loops = std::size(directions);

    for (int i = 0; i < num_loops; i++) {
        Position new_pos = pos;
        new_pos.first += directions[i][0];
        new_pos.second += directions[i][1];
        // if the position isnt in the range
        if (new_pos.first < 1 || new_pos.first > BOARD_WIDTH || new_pos.second > BOARD_HEIGHT || new_pos.second < 1) continue;
        if (are_opposite_colors(m_board->get_piece(new_pos), p)) {
            legal_moves->push_back(Move(p, pos, new_pos, true));
        } else if (m_board->get_piece(new_pos) == NO_PIECE) {
            legal_moves->push_back(Move(p, pos, new_pos));
        }

    }
}


Engine::~Engine() {
    delete m_board;
}


