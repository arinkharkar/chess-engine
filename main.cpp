#include "main.h"
#include "renderer.h"
#include "engine.h"

int main(int argc, char* argv[]) {
    if (gui_enabled(argc, argv)) {
        fprintf(stderr, "ERROR: GUI not supported currently");
        exit(1);
    }
    engine* eng = new engine();
    eng->print_board();
    return 0;
    eng->get_legal_moves();
}


bool gui_enabled(int argc, char** argv) {
    if (argc < 2) {
        return false;
    }

    if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
        printf("Kngine v 0.0.1, Estimated ELO: 0");
        exit(0);
    } else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        printf("USAGE: chess-engine [-g for GUI]");
        exit(0);
    } else if (!strcmp(argv[1], "-g") || !strcmp(argv[1], "--gui")) {
        return true;
    }
    return false;
}