#include "main.h"

#include <iostream>
#include <bits/ostream.tcc>

#include "renderer.h"
#include "Engine.h"

int main(int argc, char* argv[]) {
    if (gui_enabled(argc, argv)) {
        fprintf(stderr, "ERROR: GUI not supported currently");
        exit(1);
    }
    Engine* eng = new Engine();
    eng->print_board();
    Move m = Move(eng->get_board(), Position(5,1), Position(1, 1));
    eng->make_move(m);
    eng->print_board();
 //   std::vector<Move> moves = eng->get_legal_moves();
   // std::cout << "Found " << moves.size() << " legal moves" << std::endl;
    //for (const Move& move : moves) {
       // if (move.p == BISHOP)
    //    std::cout << "moving " << to_string(move.piece) << " from: " << move.start.first << ", " << move.start.second << "\nto: " << move.end.first << ", " << move.end.second << " capture: " << move.is_capture  << std::endl;
    //}
    //move m = move(eng->get_board(), std::make_pair(4,6), std::make_pair(7,6));
    //std::cout << "r: " << eng->results_in_check(m) << std::endl;
    //eng->print_board();
}


bool gui_enabled(int argc, char** argv) {
    if (argc < 2) {
        return false;
    }

    if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
        printf("Kngine v 0.0.1, Estimated ELO: 0");
        exit(0);
    } else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        printf("USAGE: %s [-g for GUI]", argv[0]);
        exit(0);
    } else if (!strcmp(argv[1], "-g") || !strcmp(argv[1], "--gui")) {
        return true;
    }
    return false;
}