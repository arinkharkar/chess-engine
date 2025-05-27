//
// Created by arin on 5/25/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include "Board.h"

constexpr int WINDOW_HEIGHT = 900;
constexpr int WINDOW_WIDTH = 900;

struct color {
    int r, g, b, a;
};

constexpr color color_black = {0, 0, 0, 255};
constexpr color color_white = {255, 255, 255, 255};

class Board_Renderer {
    Board* m_board;
    SDL_Event m_event;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Window* m_window = nullptr;

    void init_sdl();

public:
    explicit Board_Renderer(Board* b);

    void start();

    void draw_board() const;
    [[nodiscard]] Board* get_board() const;
};



#endif //RENDERER_H
