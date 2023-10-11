#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define PLAYING_FIELD_WIDTH 300
#define PLAYING_FIELD_HEIGHT 500
#define BLOCK_SIZE (PLAYING_FIELD_HEIGHT / 20)

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
}
GAME_t;

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
}
RGBA_t;

typedef struct
{
    bool is_block;
    RGBA_t color;
    uint16_t pos_x;
    uint16_t pos_y;
}
BLOCK_t;

extern GAME_t game;

extern bool game_closed;

extern float delta_time;

extern BLOCK_t playing_field[20][10];

