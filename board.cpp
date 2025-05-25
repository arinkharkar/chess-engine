//
// Created by arin on 5/25/25.
//

#include "board.h"

#include <iostream>

board::board() : board(FEN_DEFAULT) {}

board::board(const std::string &fen) {
    // parse a FEN string
    int curX = 0;
    int curY = 0;
    for (int i = 0; i < fen.length(); i++) {
        switch (fen[i]) {
            // if theres a slash, its the end of the line
            case '/':
                curY++;
                break;
            case 'r':
                m_board[curX][curY] = ROOK_BLACK;
                break;
            case 'n':
                m_board[curX][curY] = KNIGHT_BLACK;
                break;
            case 'b':
                m_board[curX][curY] = BISHOP_BLACK;
                break;
            case 'q':
                m_board[curX][curY] = QUEEN_BLACK;
                break;
            case 'k':
                m_board[curX][curY] = KING_BLACK;
                break;
            case 'p':
                m_board[curX][curY] = PAWN_BLACK;
                break;
            case 'R':
                m_board[curX][curY] = ROOK_WHITE;
                break;
            case 'N':
                m_board[curX][curY] = KNIGHT_WHITE;
                break;
            case 'B':
                m_board[curX][curY] = BISHOP_WHITE;
                break;
            case 'Q':
                m_board[curX][curY] = QUEEN_WHITE;
                break;
            case 'K':
                m_board[curX][curY] = KING_WHITE;
                break;
            case 'P':
                m_board[curX][curY] = PAWN_WHITE;
                break;
            default:
                if (fen[i] >= '0' && fen[i] <= '9') {
                    curX += fen[i] - '0';
                } else {
                    std::cerr << "ERROR: Invalid FEN: " << fen;
                    exit(1);
                }
        }
        if (curX < BOARD_WIDTH - 1)
            curX++;
        else
            curX = 0;
        // if we have reached the end of the board, break
        if (curX == BOARD_WIDTH - 1 && curY == BOARD_HEIGHT - 1)
            break;
    }

    m_turn = turn::white;
    is_check = false;
    is_checkmate = false;
    white_can_castle = false;
    black_can_castle = false;
}

void board::print() {
    std::string board_str;
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = BOARD_WIDTH-1; x >= 0; x--) {
            if (x == BOARD_WIDTH - 1) {
                board_str += (char('8' - y));
                board_str += ' ';
            }
            bool white = false;
            // if the piece is white
            if (m_board[x][y] & WHITE)
                white = true;

            // unset the white bit, we already know if the piece is white or not
            switch (m_board[x][y] & ~WHITE) {
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
                    fprintf(stderr, "Invalid Board State: %d piece found!", m_board[x][y] & ~WHITE);
                    exit(EXIT_FAILURE);
            }

            board_str += ' ';
        }
        board_str += '\n';
        if (y == BOARD_HEIGHT-1)
            board_str += "  a b c d e f g h\n";
    }
    std::cout << board_str << std::endl;
}

void board::reset_board() {
    board(FEN_DEFAULT);
}

piece board::get_piece(position pos) const {
    return m_board[pos.first][pos.second];
}

piece board::get_piece(int x, int y) const {
    return m_board[x][y];
}

void board::place_piece(piece placed_piece, position pos) {
    m_board[pos.first][pos.second] = placed_piece;
}

void board::place_piece(const piece placed_piece, const int x, const int y) {
    m_board[x][y] = placed_piece;
}

