// output.c

// Rendering of the program.

#include "include/var.h"
#include "include/output.h"

// Initializes the game window, renderer and local variables
extern void init_SDL_video(void)
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Could not initialize SDL_Video: %s\n", SDL_GetError());
        exit(1);
    }

    game.window = SDL_CreateWindow("TetriC", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!game.window)
    {
        printf("Could not create game winow: %s\n", SDL_GetError());
        exit(1);
    }

    game.renderer = SDL_CreateRenderer(game.window, -0, SDL_RENDERER_ACCELERATED);

    if (!game.renderer)
    {
        printf("Could not create game renderer: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
}

// Renders the current frame
extern void update_window(void)
{
    SDL_RenderClear(game.renderer);

    SDL_RenderPresent(game.renderer);
}

// Unloads video subsystem and resources used
extern void unload_SDL_video(void)
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}