// input.c

// Handles program inputs.

#include "include/var.h"
#include "include/input.h"
#include "include/game.h"

extern void handle_input(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:

                game.is_closed = true;
                break;
        
            case SDL_KEYDOWN:

                switch (event.key.keysym.sym)
                {
                    case (SDLK_a):

                        if (active_piece.cord_x == 1) {continue;}

                        active_piece.move(&active_piece, LEFT);
                        break;
                    
                    case (SDLK_d):

                        if (active_piece.cord_x == 10) {continue;}

                        active_piece.move(&active_piece, RIGHT);
                        break;
                }

            default:
                break;
        }
    }
}