// input.c

// Handles program inputs.

#include "include/var.h"
#include "include/input.h"

extern void handle_input(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:

            game_closed = true;
                break;
        
            default:
                break;
        }
    }
}