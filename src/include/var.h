#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 540
#define WINDOW_HEIGHT 720

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    bool is_closed;           // True when game is running and set to false when SDL_Quit event is called
}
Game_t;

extern Game_t game;                 // Currently stores the window and renderer pointers

extern float delta_time;            // Time since last frame rendered  // Block array