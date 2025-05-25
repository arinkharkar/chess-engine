//
// Created by arin on 5/25/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include "board.h"

constexpr int WINDOW_HEIGHT = 900;
constexpr int WINDOW_WIDTH = 900;


class board_renderer {
    board* m_board;
    SDL_Event m_event;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Window* m_window = nullptr;

    void init_sdl();

public:
    explicit board_renderer(board* b);

    void start();

    void draw_board() const;
    [[nodiscard]] board* get_board() const;
};



#endif //RENDERER_H
