// output.c

// Rendering of the program.

#include "include/var.h"
#include "include/output.h"
#include "include/log.h"
#include "include/game.h"

#define PLAYING_FIELD_WIDTH 300
#define PLAYING_FIELD_HEIGHT 500
#define BLOCK_SIZE (PLAYING_FIELD_HEIGHT / 20)          // Size of playing field blocks
#define BLOCK_PADDING 3

static void render_block_array(const uint16_t field_x, const uint16_t field_y)
{
    assert(field_x <= WINDOW_WIDTH && field_y <= WINDOW_HEIGHT);

    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);

    for (int i = 2; i < 22; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (!playing_field[i][j].is_block)
            {
                continue;
            }

            SDL_Rect block_rect = 
            {
            .h = BLOCK_SIZE - BLOCK_PADDING,
            .w = BLOCK_SIZE - BLOCK_PADDING,
            .x = field_x + (j * BLOCK_SIZE),
            .y = field_y + i * BLOCK_SIZE - BLOCK_SIZE * 2
            };

            SDL_RenderFillRect(game.renderer, &block_rect);

            SDL_RenderDrawRect(game.renderer, &block_rect);
        }
    }

    return;
}

static void render_playing_field(uint16_t pos_x, uint16_t pos_y)
{
    assert(pos_x <= WINDOW_WIDTH && pos_y <= WINDOW_HEIGHT);

    const uint16_t offset_y = 50;

    SDL_Rect rect = {.h = PLAYING_FIELD_HEIGHT, .w = PLAYING_FIELD_WIDTH, .x = pos_x, .y = pos_y + offset_y};

    render_block_array(pos_x, pos_y + offset_y);

    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);

    SDL_RenderDrawRect(game.renderer, &rect);

    return;
}

extern void init_SDL_video(void)
{
    game.window = SDL_CreateWindow("TetriC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!game.window)
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

    if (!game.renderer)
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    write_log("Game renderer and window created", LOG_OUT_FILE | LOG_TYPE_INF);

    return;
}

extern void render_frame(void)
{
    SDL_RenderClear(game.renderer);

    render_playing_field(WINDOW_WIDTH / 2 - PLAYING_FIELD_WIDTH / 2, WINDOW_HEIGHT / 2 - PLAYING_FIELD_HEIGHT / 2);

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);

    SDL_RenderPresent(game.renderer);

    return;
}

extern void unload_SDL_video(void)
{
    SDL_DestroyRenderer(game.renderer);

    SDL_DestroyWindow(game.window);

    write_log("Game window and renderer destroyed", LOG_OUT_FILE | LOG_TYPE_INF);

    return;
}