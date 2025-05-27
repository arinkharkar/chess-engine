//
// Created by arin on 5/25/25.
//


#include "renderer.h"


Board_Renderer::Board_Renderer(Board *b) {
    m_board = b;
    printf("her");
    init_sdl();
}


void Board_Renderer::init_sdl() {
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

void Board_Renderer::start() {
    while (true) {
        if (SDL_PollEvent(&m_event) && m_event.type == SDL_QUIT)
            break;
        draw_board();
        for (int i = 0; i < BOARD_WIDTH; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
              //  draw_piece(i, j, m_board[i][j]);
            }
        }
        SDL_RenderPresent(m_renderer);
    }
}

void Board_Renderer::draw_board() const {
    constexpr int tile_width = WINDOW_WIDTH / BOARD_WIDTH;
    constexpr int tile_height = WINDOW_HEIGHT / BOARD_HEIGHT;
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            // Alternate colors
            if ((x + y) % 2 != 0)
                SDL_SetRenderDrawColor(m_renderer, color_black.r, color_black.g, color_black.b, color_black.a); // White
            else
                SDL_SetRenderDrawColor(m_renderer, color_white.r, color_white.g, color_white.b, color_white.a);       // Black

            SDL_Rect tile_rect = {
                x * tile_width,
                y * tile_height,
                tile_width,
                tile_height
            };
            SDL_RenderFillRect(m_renderer, &tile_rect);
        }
    }
}

Board* Board_Renderer::get_board() const {
    return m_board;
}


