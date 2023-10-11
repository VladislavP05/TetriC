// tetric.c

// The entry to the program and main interface between all other files. It also
// initializes and keeps in sync all functions.

#include "include/var.h"
#include "include/tetric.h"
#include "include/input.h"
#include "include/output.h"

#define FPS 75
#define TARGET_FRAME_TIME (1000 / FPS)

int main(void)
{
    initialize_SDL();

    while (!game_closed)
    {
        update_frame_time();

        handle_input();

        render_frame();
    }

    unload_resources();

    return 0;
}

static void update_frame_time(void)
{
    static uint32_t frame_time;

    uint32_t frame_delay = TARGET_FRAME_TIME - (SDL_GetTicks() - frame_time);

    if (frame_delay > 0 && frame_delay < TARGET_FRAME_TIME)
    {
        SDL_Delay(frame_delay);
    }

    delta_time = (SDL_GetTicks() - frame_time) / 1000.0f;

    frame_time = SDL_GetTicks();
}

// Initializes all program subsystems and file variables
static void initialize_SDL(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER))
    {
        fprintf(stderr, "ERR: Error initializeing SDL - %s\n", SDL_GetError());
        exit(1);
    }

    init_SDL_video();
}

// Unloads all program subsystems and resources
static void unload_resources(void)
{
    unload_SDL_video();

    SDL_Quit();
}