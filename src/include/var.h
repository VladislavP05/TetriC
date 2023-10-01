#pragma once

#include <stdint.h>
#include <SDL2/SDL.h>

#define TRUE 1
#define FALSE 0

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

extern uint8_t game_closed;

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
}
game_t;

extern game_t game;
