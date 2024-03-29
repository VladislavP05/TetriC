// tetric.c

// The entry to the program and main interface between all other files. It also
// initializes and keeps in sync all functions.

#include <time.h>

#include "var.h"
#include "util.h"
#include "log.h"
#include "tetric.h"
#include "input.h"
#include "output.h"
#include "ui.h"
#include "game.h"

#define TARGET_FRAME_TIME (1000 / FPS)          // Time between each frame

int main(void)
{
    if (atexit(&unload_resources))
    {
        fprintf(stderr, "Exit function registration failed\n");
        return 1;
    }

    srand(time(NULL));
    initialize_resources();

    while (!game.is_closed)
    {
        update_frame_time();
        upadate_log_time();
        handle_input();
        tick_logic();
        render_frame();
    }

    return 0;
}

static void update_frame_time(void)
{
    static uint32_t frame_time = 0;

    uint32_t frame_delay = TARGET_FRAME_TIME - (SDL_GetTicks() - frame_time);

    if (frame_delay > 0 && frame_delay <= TARGET_FRAME_TIME)
    {
        SDL_Delay(frame_delay);
    }

    frame_time = SDL_GetTicks();
}

static void initialize_resources(void)
{
    init_log();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER))
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    start_game();
    init_output();
}

static void unload_resources(void)
{
    unload_game();
    unload_output();
    SDL_Quit();
    unload_log();
}