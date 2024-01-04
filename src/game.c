// game.c

/*
Initializes the game playing field and piece queue. Defines the piece move, rotation functions and shapes.
Unloads the piece queue when the exit function is called.
*/
#include <stdlib.h>
#include "include/var.h"
#include "include/util.h"
#include "include/game.h"
#include "include/log.h"

// Size of the piece queue. The default is 3
#define PIECE_LIMIT 3

// Array containing the available shapes to be chosen during piece creation. The first index is the id of that shape
static const uint8_t shapes[5][3][3] = 
{
    {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
    {{0, 0, 0}, {0, 1, 0}, {1, 1, 1}},
    {{0, 0, 0}, {1, 1, 0}, {0, 1, 1}},
    {{0, 1, 0}, {0, 1, 0}, {0, 1, 1}},
    {{0, 0, 0}, {0, 1, 1}, {0, 1, 1}},
};

// Game tick counter. Resets every second (ticks % FPS)
static uint8_t ticks;

// Array containing the next pieces to be spawned. When the spawn_piece function is called, the first piece in the queue
// gets set as the active piece and removed from the queue, then the queue shifts its elements to i - 1 and initializes the last
// piece using a random shape from the shapes array.
static active_piece_t *piece_queue[PIECE_LIMIT];

// Currently moving piece
active_piece_t active_piece;

// The main game array containing the color and status of all blocks of the field
Block_t playing_field[22][12];

/*
Checks if the requested move is valid
Parameters:
this - The current active piece
*/
static uint8_t check_move(active_piece_t *this)
{
    assert(this);

    switch (this->move_direction)
    {
        case (M_DOWN):

            if (this->cord_y == 20) {return 1;}

            for (int8_t x_offset = -1; x_offset <= 1; x_offset++)
            {
                if (this->shape[2][x_offset + 1] == 1
                && playing_field[this->cord_y + 2][this->cord_x + x_offset].is_block)
                {
                    return 1;
                }
            }

            break;

        case (M_LEFT):

            if (this->cord_x == 0) {return 1;}

            for (int8_t y_offset = -1; y_offset <= 1; y_offset++)
            {
                if (this->shape[y_offset + 1][0] == 1 && this->cord_x == 1)
                {
                    return 1;
                }
                else if (this->shape[y_offset + 1][0] == 1 
                && playing_field[this->cord_y + y_offset][this->cord_x - 2].is_block)
                {
                    return 1;
                }
                else if (this->shape[y_offset + 1][1] == 1 
                && playing_field[this->cord_y + y_offset])
            }

            break;

        case (M_RIGHT):

            if (this->cord_x == 11) {return 1;}

            for (int8_t y_offset = -1; y_offset <= 1; y_offset++)
            {
                if (this->shape[y_offset + 1][2] == 1 && this->cord_x == 10)
                {
                    return 1;
                }
                else if (this->shape[y_offset + 1][2] == 1 
                && playing_field[this->cord_y + y_offset][this->cord_x + 2].is_block)
                {
                    return 1;
                }
            }

            break;
    }

    return 0;
}

/*
Moves the active piece in the specified direction
Parameters:
this - The current active piece
direction - The direction in which the piece will attempt to move. Use the enumerated values defined in game.h (example M_LEFT).
*/
static uint8_t move_piece(active_piece_t *this, uint8_t direction)
{
    // To be rewriten

    assert(this);

    this->move_direction = direction;

    switch (this->move_direction)
    {
        case (M_NONE):

            write_log("Active piece move function called with argument NONE", LOG_OUT_FILE | LOG_TYPE_WRN);
            break;

        case (M_DOWN):

            if (check_move(this))
            {
                // TEST

                for (int y_offset = 1; y_offset >= -1; y_offset--)
                {                
                    for (int x_offset = -1; x_offset <= 1; x_offset++)
                    {
                        if (playing_field[this->cord_y + y_offset][this->cord_x + x_offset].is_block)
                        {
                            playing_field[this->cord_y + y_offset][this->cord_x + x_offset].is_static = true;
                        }
                    }
                }

                return 1; //TEST
            }

            for (int y_offset = 1; y_offset >= -1; y_offset--)
            {
                for (int x_offset = -1; x_offset <= 1; x_offset++)
                {
                    if (this->shape[y_offset + 1][x_offset + 1] != 1) {continue;}

                    swap(&playing_field[this->cord_y + y_offset][this->cord_x + x_offset],
                    &playing_field[(this->cord_y + 1) + y_offset][this->cord_x + x_offset],
                    sizeof(Block_t));
                }
            }

            this->cord_y++;
            break;

        case (M_LEFT):

            if (check_move(this))
            {
                break;
            }

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

        case (M_RIGHT):

            if (check_move(this))
            {
                break;
            }

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

    return 0; //TEST
}

/*
Initializes every piece in the piece_queue and randomly chooses it a shape. Gets called at the start of
the game and every time a new piece is spawned
*/
static void populate_piece_queue()
{
    for (uint8_t i = 0; i < PIECE_LIMIT; i++)
    {
        if (piece_queue[i] != NULL) {continue;}

        piece_queue[i] = malloc(sizeof(active_piece_t));
        if (!piece_queue[i])
        {
            write_log("Piece queue memory allocation failed", LOG_OUT_BOTH | LOG_TYPE_ERR);
            exit(1);
        }
        piece_queue[i]->cord_x = piece_queue[i]->cord_y = 0;

        piece_queue[i]->shape_id = rand() % 5;
        memcpy(piece_queue[i]->shape, shapes[piece_queue[i]->shape_id], sizeof(uint8_t) * 9);
    }
}

/*
Gets the first piece in the queue and spawns it at the top of the playing field
*/
static void spawn_next_piece()
{
    // Copies the first piece of the queue into the active piece variable
    memcpy(&active_piece, piece_queue[0], sizeof(active_piece_t));

    // Shifts the pieces of the query to i - 1
    for (uint8_t i = 0; i < PIECE_LIMIT - 1; i++)
    {
        memcpy(piece_queue[i], piece_queue[i + 1], sizeof(active_piece_t));
    }
    free(piece_queue[PIECE_LIMIT - 1]);
    piece_queue[PIECE_LIMIT - 1] = NULL;

    populate_piece_queue();

    active_piece.cord_x = 5; active_piece.cord_y = 2;
    active_piece.move_direction = M_NONE;
    active_piece.move = move_piece;

    for (int y_offset = 0; y_offset < 3; y_offset++)
    {
        for (int x_offset = 0; x_offset < 3; x_offset++)
        {
            if (!active_piece.shape[y_offset][x_offset]) {continue;}
            playing_field[2 + (y_offset - 1)][5 + (x_offset - 1)].color = set_color(255, 255, 255, 255);
            playing_field[2 + (y_offset - 1)][5 + (x_offset - 1)].is_block = true;
        }
    }
}

/*
Unloads the piece queue at the end of the game
*/
static void unload_piece_queue()
{
    for (uint8_t i = 0; i < PIECE_LIMIT; i++)
    {
        if (piece_queue[i] == NULL) {continue;}
        free(piece_queue[i]);
    }

    write_log("Piece queue unloaded", LOG_TYPE_INF | LOG_OUT_FILE);
}

/*
Increments the tick counter and handles any game logic this frame
*/
extern void tick_logic(void)
{
    // Test

    ticks++;

    if (!(ticks % FPS == 0)) {return;}

    if (active_piece.move(&active_piece, M_DOWN))
    {
        spawn_next_piece(); // TEST
    }

    ticks = 0;

    return;
}

/*
Initializes the playing field and piece queue. Called once at the start
*/
extern void start_game(void)
{
    // Test implementation

    static_assert(PIECE_LIMIT < 256, "Piece limit should be lower than 256");

    write_log("Starting game...", LOG_TYPE_INF | LOG_OUT_FILE);

    for (int i = 0; i < 22; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            playing_field[i][j].is_block = false;
            playing_field[i][j].is_static = false;
            playing_field[i][j].color = set_color(0, 0, 0, 0);
        }
    }

    populate_piece_queue();
    write_log("Piece queue initialized", LOG_TYPE_INF | LOG_OUT_FILE);
    spawn_next_piece();
}

/*
Calls the unload_piece_queue function, writing a log if it unloads successfully
*/
extern void unload_game(void)
{
    unload_piece_queue();
    write_log("Game sucesfully unloaded", LOG_TYPE_INF | LOG_OUT_FILE);
}