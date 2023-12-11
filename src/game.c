// game.c

// Game logic
#include <stdlib.h>
#include "include/var.h"
#include "include/util.h"
#include "include/game.h"
#include "include/log.h"

#define ACTIVE_PIECE_LIMIT 3

static uint8_t ticks;

active_piece_t active_piece;

static void move_active_piece(active_piece_t *this, uint8_t direction)
{
    assert(this);

    this->curr_move_direcrion = direction;

    switch (this->curr_move_direcrion)
    {
        case (NONE):

            write_log("Active piece move function called with argument NONE.", LOG_OUT_FILE | LOG_TYPE_WRN);
            break;

        case (DOWN):

            for (int y_offset = 1; y_offset >= -1; y_offset--)
            {
                for (int x_offset = -1; x_offset <= 1; x_offset++)
                {
                    swap(&playing_field[this->cord_y + y_offset][this->cord_x + x_offset],
                            &playing_field[(this->cord_y + 1) + y_offset][this->cord_x + x_offset],
                                sizeof(Block_t));
                }
            }

            this->cord_y++;
            break;

        case (LEFT):

            for (int y_offset = 1; y_offset >= -1; y_offset--)
            {
                for (int x_offset = -1; x_offset <= 1; x_offset++)
                {
                    swap(&playing_field[this->cord_y + y_offset][this->cord_x + x_offset],
                            &playing_field[this->cord_y + y_offset][(this->cord_x - 1) + x_offset],
                                sizeof(Block_t));
                }
            }

            this->cord_x--;
            break;

        case (RIGHT):

            for (int y_offset = 1; y_offset >= -1; y_offset--)
            {
                for (int x_offset = 1; x_offset >= -1; x_offset--)
                {
                    swap(&playing_field[this->cord_y + y_offset][this->cord_x + x_offset],
                            &playing_field[this->cord_y + y_offset][(this->cord_x + 1) + x_offset],
                                sizeof(Block_t));
                }
            }

            this->cord_x++;
            break;
    }
}

static void init_active_piece(active_piece_t *this, const uint8_t active_piece_num)
{
    if (active_piece_num >= ACTIVE_PIECE_LIMIT)
    {
        return;
    }

    this->cord_x = 5; this->cord_y = 2;
    this->curr_move_direcrion = NONE;
    this->move = move_active_piece;
    this->next_piece = malloc(sizeof(active_piece_t));
    init_active_piece(this->next_piece, active_piece_num++);
}

Block_t playing_field[22][12];

extern void tick_logic(void)
{
    // Test

    ticks++;

    if (!(ticks % 75 == 0) || active_piece.cord_y == 20) {return;}

    active_piece.move(&active_piece, DOWN);

    ticks = 0;

    return;
}

extern void start_game(void)
{
    // Test implementation

    for (int i = 0; i < 22; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            playing_field[i][j].is_block = false;
            playing_field[i][j].is_static = false;
            playing_field[i][j].color = set_color(0, 0, 0, 0);
        }
    }

    playing_field[2][5].color = set_color(255, 255, 255, 255);
    playing_field[3][4].color = set_color(255, 255, 255, 255);
    playing_field[3][5].color = set_color(255, 255, 255, 255);
    playing_field[3][6].color = set_color(255, 255, 255, 255);
    playing_field[2][5].is_block = true;
    playing_field[3][4].is_block = true;
    playing_field[3][5].is_block = true;
    playing_field[3][6].is_block = true;

    init_active_piece(&active_piece, 0);
}