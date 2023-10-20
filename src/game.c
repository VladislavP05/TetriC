// game.c

// Game logic

#include "include/var.h"
#include "include/util.h"
#include "include/game.h"

static uint8_t ticks;

extern void tick_logic(void)
{
    // Test

    ticks++;

    if (!(ticks % 15 == 0)) {return;}

    for (int i = 0; i < 22; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (!playing_field[i][j].is_block || i == 21) {continue;}

            swap(&playing_field[i][j], &playing_field[i + 1][j], sizeof(BLOCK_t));

            goto end;
        }
    }

end:
    ticks = 0;

    return;
}

extern void start_game(void)
{
    init_playing_field();
}