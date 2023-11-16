// tetric.c

// The entry to the program and main interface between all other files. It also
// initializes and keeps in sync all functions.

#include "include/var.h"
#include "include/util.h"
#include "include/log.h"
#include "include/tetric.h"
#include "include/input.h"
#include "include/output.h"
#include "include/game.h"

#define FPS 75          // Target frames per second
#define TARGET_FRAME_TIME (1000 / FPS)          // Time between each frame

int main(void)
{
    initialize_resources();

    while (!game.is_closed)
    {
        update_frame_time();

        upadate_log_time();

        handle_input();

        tick_logic();

        render_frame();
    }

    unload_resources();

    return 0;
}

static void update_frame_time(void)
{
    static uint32_t frame_time;

    uint32_t frame_delay = TARGET_FRAME_TIME - (SDL_GetTicks() - frame_time);

    printf("Frame delay: %u\n", frame_delay);

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

    init_SDL_video();
    start_game();
}

static void unload_resources(void)
{
    unload_SDL_video();
    SDL_Quit();
    unload_log();
}