// output.c

// Rendering of the program.

#include "include/var.h"
#include "include/output.h"

static uint8_t render_block_array(const uint16_t field_x, const uint16_t field_y)
{
    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            // if (playing_field[i][j].is_block == false) {continue;}

            SDL_Rect block_rect = {.h = BLOCK_SIZE - 1, .w = BLOCK_SIZE - 1, .x = field_x + (j * BLOCK_SIZE), .y = field_y + i * BLOCK_SIZE};

            SDL_RenderFillRect(game.renderer, &block_rect);

            SDL_RenderDrawRect(game.renderer, &block_rect);
        }
    }

    return 0;
}

static void render_playing_field(uint16_t pos_x, uint16_t pos_y)
{
    const uint16_t offset_y = 50;

    SDL_Rect rect = {.h = PLAYING_FIELD_HEIGHT, .w = PLAYING_FIELD_WIDTH, .x = pos_x, .y = pos_y + offset_y};

    render_block_array(pos_x, pos_y + offset_y);

    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);

    SDL_RenderDrawRect(game.renderer, &rect);
}

// Initializes the game window, renderer and local variables
extern void init_SDL_video(void)
{
    game.window = SDL_CreateWindow("TetriC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!game.window)
    {
        fprintf(stderr,"ERR: Error creating game winow: %s\n", SDL_GetError());
        exit(1);
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

    if (!game.renderer)
    {
        fprintf(stderr, "ERR: Error creating game renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

// Renders the current frame
extern void render_frame(void)
{
    SDL_RenderClear(game.renderer);

    render_playing_field(WINDOW_WIDTH / 2 - PLAYING_FIELD_WIDTH / 2, WINDOW_HEIGHT / 2 - PLAYING_FIELD_HEIGHT / 2);

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);

    SDL_RenderPresent(game.renderer);
}

// Unloads resources used by the video system including the renderer and window
extern void unload_SDL_video(void)
{
    SDL_DestroyRenderer(game.renderer);

    SDL_DestroyWindow(game.window);
}