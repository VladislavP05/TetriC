// tetric.c

// The entry to the program and main interface between all other files. It also
// initializes and keeps in sync all functions.

#include "include/var.h"
#include "include/tetric.h"
#include "include/input.h"
#include "include/output.h"

int main(void)
{
    memset(&game, 0, sizeof(game));

    initialize_SDL();

    while (!game_closed)
    {
        handle_input();

        update_window();
    }
    return 0;
}

// Initializes all program subsystems and file variables
static void initialize_SDL(void)
{
    init_SDL_video();
}

// Unloads all program subsystems and resources
static void unload_game()
{
    unload_SDL_video();

    SDL_Quit();
}