#include "renderer.h"

int main() {
    board* b = new board();
    board_renderer* renderer = new board_renderer(b);

    renderer->start();
}