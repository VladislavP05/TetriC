#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    bool is_closed;           // True when game is running and set to false when SDL_Quit event is called
}
Game_t;

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
}
Rgba_t;

extern Game_t game;                 // Currently stores the window and renderer pointers

extern float delta_time;            // Time since last frame rendered  // Block array