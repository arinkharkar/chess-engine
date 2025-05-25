//
// Created by arin on 5/25/25.
//


#include "renderer.h"


board_renderer::board_renderer(board *b) {
    m_board = b;
    printf("her");
    init_sdl();
}


void board_renderer::init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO))
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &m_window, &m_renderer);

    if (!m_window || !m_renderer) {
        fprintf(stderr, "Could not initialize SDL2: %s\n", SDL_GetError());
    }


    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);


}

void board_renderer::start() {
    while (true) {
        if (SDL_PollEvent(&m_event) && m_event.type == SDL_QUIT)
            break;
        draw_board();
    }
}

void board_renderer::draw_board() const {
    // Create checkerboard
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            if (x / WINDOW_HEIGHT)
        }
    }
}

board* board_renderer::get_board() const {
    return m_board;
}


