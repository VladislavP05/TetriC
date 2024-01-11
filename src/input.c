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

                        active_piece.move(&active_piece, M_LEFT);
                        break;
                    
                    case (SDLK_d):

                        active_piece.move(&active_piece, M_RIGHT);
                        break;

                    case(SDLK_q):

                        active_piece.rotate(&active_piece, R_CLOCKWISE);
                        break;
                    
                    case(SDLK_e):

                        active_piece.rotate(&active_piece, R_CONTCLOCKWISE);
                        break;
                }
        }
    }
}