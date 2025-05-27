//
// Created by arin on 5/25/25.
//

#include "Board.h"

#include <bitset>
#include <cstring>
#include <iostream>

Board::Board() : Board(FEN_DEFAULT) {}

Board::Board(const std::string &fen) {
    // parse a FEN string


    // first, reset the board
    for (int i = 0; i < BOARD_HEIGHT+1; i++) {
        for (int j = 0; j < BOARD_WIDTH+1; j++) {
            m_board[j][i] = NO_PIECE;
        }
    }
    int curX = 1;       // File (a–h), starts at 1
    int curY = BOARD_HEIGHT;       // Rank (8–1), starts at 8
    size_t i;
    for (i = 0; i < fen.length(); i++) {
        char p = fen[i];

        // Handle the numbers here, they do not belong in the switch
        if (p >= '1' && p <= '8') {
            curX += p - '0';
        } else if (p == ' ') {
            break;
        } else {
            switch (p) {
                case '/': curY--; curX=1; break;
                case 'r': m_board[curX][curY] = ROOK_BLACK; curX++; break;
                case 'n': m_board[curX][curY] = KNIGHT_BLACK; curX++; break;
                case 'b': m_board[curX][curY] = BISHOP_BLACK; curX++; break;
                case 'q': m_board[curX][curY] = QUEEN_BLACK; curX++; break;
                case 'k': m_board[curX][curY] = KING_BLACK; curX++; break;
                case 'p': m_board[curX][curY] = PAWN_BLACK; curX++; break;
                case 'R': m_board[curX][curY] = ROOK_WHITE; curX++; break;
                case 'N': m_board[curX][curY] = KNIGHT_WHITE; curX++; break;
                case 'B': m_board[curX][curY] = BISHOP_WHITE; curX++; break;
                case 'Q': m_board[curX][curY] = QUEEN_WHITE; curX++; break;
                case 'K': m_board[curX][curY] = KING_WHITE; curX++; break;
                case 'P': m_board[curX][curY] = PAWN_WHITE; curX++; break;
                default: std::cerr << "Unknown piece in FEN string: '" << p << "'" << std::endl; exit(1); break;
            }
        }
    }

    // After loop: ensure board is complete
    if (curY != 1 || curX != 9) {
        std::cerr << "FEN parsing incomplete: did not fill exactly 8x8 board.\n";
        exit(1);
    }




    /* now parse the rest of the FEN */
    // Check the FEN is long enough
    // if the next char is a 'w', it is whites turn to move
    end_loop:
    i++;

    if (i >= fen.size()) {
        std::cerr << "ERROR: Invalid FEN: " << fen;
        exit(1);
    }
    if (fen[i] == 'w')
        m_turn = Turn::white;
    else if (fen[i] == 'b')
        m_turn = Turn::black;
    else {
        std::cerr << "ERROR: Invalid FEN: " << fen;
        exit(1);
    }

    i+=2;
    // check the Queen and King castling rights
    while (fen[i] != ' ') {
        if (i == fen.size()) {
            std::cerr << "ERROR: Invalid FEN: " << fen;
            exit(1);
        }
        switch (fen[i]) {
            case 'Q':
                m_white_queen_castle = true;
                break;
            case 'q':
                m_black_queen_castle = true;
                break;
            case 'K':
                m_white_king_castle = true;
                break;
            case 'k':
                m_black_king_castle = true;
                break;
            case '-':
                break;
            default:
                std::cerr << "ERROR: Invalid FEN: " << fen;
                exit(1);
        }
        i++;
    }

    i++;

    if (fen[i] == '-') {
        // invalid position, since no en passant
        m_en_passant_position = std::make_pair(0, 0);
    } else {
        if (fen[i] <'a' || fen[i] > 'h') {
            std::cerr << "ERROR: Invalid FEN: " << fen;
            exit(1);
        }
        int x_coord = fen[i] - 'a' + 1;
        i++;
        if (fen[i] < '0' || fen[i] > '8') {
            std::cerr << "ERROR: Invalid FEN: " << fen;
            exit(1);
        }
        int y_coord = fen[i] - '0' + 1;
        std::cout << "En Passant at: " << x_coord << ", " << y_coord << std::endl;
    }
    i+=2;
    // now get the halfmove number
    std::string halfmove_number_str;
    while (fen[i] != ' ') {
        if (i == fen.size()) {
            std::cerr << "ERROR: Invalid FEN: " << fen;
            exit(1);
        }
        halfmove_number_str += fen[i];
        i++;
    }

    try {
        m_halfmove_number = std::stoi(halfmove_number_str);
    } catch (std::invalid_argument& e) {
        std::cerr << "ERROR: Invalid FEN: " << fen;
        exit(1);
    }
    i++;
    // finally, get the fullmove number
    std::string fullmove_number_str;
    while (fen[i] != ' ') {
        if (i == fen.size()) {
            // we reached the end of the string
            break;
        }
        fullmove_number_str += fen[i];
        i++;
    }

    try {
        m_fullmove_number = std::stoi(fullmove_number_str);
    } catch (std::invalid_argument& e) {
        std::cerr << "ERROR: Invalid FEN: " << fen;
        exit(1);
    }
}

void Board::print() {
    std::string board_str;
    for (int y = BOARD_HEIGHT; y > 0; y--) {
        for (int x = 1; x <= BOARD_WIDTH; x++) {
            // print the row
            if (x == 1) {
                board_str += (char('0' + y));
                board_str += ' ';
            }
            bool white = false;

            Piece p;

            // if the piece is white
            if (m_board[x][y] & WHITE) {
                white = true;
                p = Piece(m_board[x][y] & ~WHITE);
                // if the piece is black
            } else if (m_board[x][y] & BLACK) {
                white = false;
                p = Piece(m_board[x][y] & ~BLACK);
            } else if (m_board[x][y] != NO_PIECE) {
                std::cerr << "ERROR: No color attatched to piece: " << m_board[x][y];
            } else {
                p = NO_PIECE;
            }

            // unset the white bit, we already know if the piece is white or not
            switch (p) {
                case PAWN:
                    if (white)
                        board_str += 'P';
                    else
                        board_str += 'p';
                    break;
                case ROOK:
                    if (white)
                        board_str += 'R';
                    else
                        board_str += 'r';
                    break;
                case KNIGHT:
                    if (white)
                        board_str += 'N';
                    else
                        board_str += 'n';
                    break;
                case BISHOP:
                    if (white)
                        board_str += 'B';
                    else
                        board_str += 'b';
                    break;
                case QUEEN:
                    if (white)
                        board_str += 'Q';
                    else
                        board_str += 'q';
                    break;
                case KING:
                    if (white)
                        board_str += 'K';
                    else
                        board_str += 'k';
                    break;
                case NO_PIECE:
                    board_str += 'x';
                    break;
                default:
                    board_str += m_board[x][y] & ~WHITE;
                    break;
                    fprintf(stderr, "Invalid Board State: %d found at %d %d!\n", m_board[x][y] & ~WHITE, x, y);
                    fprintf(stderr, "Current board string: \n%s", board_str.c_str());
                    exit(EXIT_FAILURE);
            }

            board_str += ' ';
        }
        board_str += '\n';
        // if we have reached the last rank (1), add the file letters to the end
        if (y == 1)
            board_str += "  A B C D E F G H\n";
    }


    if (m_black_king_castle)
        board_str += "Black King castle\n";
    if (m_black_queen_castle)
        board_str += "Black Queen castle\n";
    if (m_white_king_castle)
        board_str += "White King castle\n";
    if (m_white_queen_castle)
        board_str += "White Queen castle\n";
    if (m_turn == Turn::white)
        board_str += "White to move\n";
    else if (m_turn == Turn::black)
        board_str += "Black to move\n";
    if (m_is_check)
        board_str += "Check\n";
    if (m_is_checkmate)
        board_str += "Checkmate\n";
    board_str += "number of halfmoves: " + std::to_string(m_halfmove_number) + "\n";

    std::cout << board_str << std::endl;
}

bool Board::in_bounds(int x, int y) {
    if (x < 1 || x > BOARD_WIDTH || y < 1 || y > BOARD_HEIGHT)
        return false;
    return true;
}

bool Board::in_bounds(Position pos) {
    if (pos.first < 1 || pos.first > BOARD_WIDTH || pos.second < 1 || pos.second > BOARD_HEIGHT)
        return false;
    return true;
}


void Board::reset_board() {
    Board(FEN_DEFAULT);
}

Piece Board::get_piece(Position pos) const {
    return m_board[pos.first][pos.second];
}

Piece Board::get_piece(int x, int y) const {
    return m_board[x][y];
}

void Board::place_piece(Piece placed_piece, Position pos) {
    m_board[pos.first][pos.second] = placed_piece;
}

void Board::place_piece(const Piece placed_piece, const int x, const int y) {
    m_board[x][y] = placed_piece;
}

Position Board::find_piece(Piece p) const {
    for (int y = BOARD_HEIGHT; y > 0; y--) {
        for (int x = 1; x <= BOARD_WIDTH; x++) {
            if (m_board[x][y] == p) {
                return std::make_pair(x, y);
            }
        }
    }
    return std::make_pair(-1, -1);
}


Turn Board::get_turn() const {
    return m_turn;
}
