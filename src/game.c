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

// Array containing the available shapes to be chosen during piece creation. The first index is the id of that shape
static const uint8_t shapes[PIECE_SHAPES][3][3] = 
{
    {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
    {{0, 0, 0}, {0, 1, 0}, {1, 1, 1}},
    {{0, 0, 0}, {1, 1, 0}, {0, 1, 1}},
    {{0, 0, 0}, {0, 1, 1}, {1, 1, 0}},
    {{0, 1, 0}, {0, 1, 0}, {0, 1, 1}},
    {{0, 1, 0}, {0, 1, 0}, {1, 1, 0}},
    {{0, 0, 0}, {0, 1, 1}, {0, 1, 1}}
};

// Game tick counter. Resets every second (ticks % FPS)
static uint8_t ticks;
static SDL_Color shapes_color[PIECE_SHAPES];
static float score_multiplier = 1.0f;

// Array containing the next pieces to be spawned. When the spawn_piece function is called, the first piece in the queue
// gets set as the active piece and removed from the queue, then the queue shifts its elements to i - 1 and initializes the last
// piece using a random shape from the shapes array.
active_piece_t *piece_queue[QUEUE_LIMIT];

// Currently moving piece
active_piece_t active_piece;

// The main game array containing the color and status of all blocks of the field
Block_t playing_field[22][12];

uint32_t score = 0;

static void init_shape_color(void)
{
    uint8_t colors[PIECE_SHAPES][4] = 
    {
        {0, 170, 170, 255},
        {100, 0, 170, 255},
        {0, 170, 0, 255},
        {170, 0, 0, 255},
        {170, 100, 0, 255},
        {0, 0, 170, 255},
        {170, 170, 0, 255}
    };

    for (uint8_t i = 0; i < PIECE_SHAPES; i++)
    {
        shapes_color[i] = set_color(colors[i][0], colors[i][1], colors[i][2], colors[i][3]);
    }
}

static void clear_lines(uint8_t line_idx, uint8_t num_lines)
{
    memset(playing_field[line_idx], 0, (sizeof(Block_t) * 12) * num_lines);

    for (uint8_t i = 0; i < line_idx; i++)
    {
        memcpy(playing_field[(line_idx + num_lines - 1) - i], playing_field[(line_idx - 1) - i], sizeof(Block_t) * 12);
        memset(playing_field[(line_idx - 1) - i], 0, sizeof(Block_t) * 12);
    }

    return;
}

static uint8_t count_lines(uint8_t line_idx)
{
    uint8_t count = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
        for (uint8_t j = 0; j < 12; j++)
        {
            if (!playing_field[line_idx + i][j].is_block)
            {
                return count;
            }
        }

        count++;
    }

    return count;
}

static void check_lines(void)
{
    uint8_t line_num = 0;
    uint8_t line_idx = 0;

    for (uint8_t i = 1; i <= 21 && line_num == 0; i++)
    {
        line_idx = i;
        line_num = count_lines(line_idx);
    }

    if (line_num == 0)
    {
        return;
    }

    score += (50 * score_multiplier) * line_num;

    clear_lines(line_idx, line_num);

    return;
}

/*
Checks if the requested move is valid. Returns a 1 if the move is not allowed
Parameters:
this - The current active piece
*/
static uint8_t check_move(active_piece_t *this)
{
    assert(this);

    switch (this->move_direction)
    {
        case(M_NONE):

            return 1;

        case (M_DOWN):

            if (this->cord_y == 21) {return 1;}

            for (int8_t x_offset = -1; x_offset < 2; x_offset++)
            {
                if (this->shape[2][x_offset + 1] == 1 && this->cord_y == 20)
                {
                    return 1;
                }

                for (int8_t y_offset = 1; y_offset > -2; y_offset--)
                {
                    if (this->shape[y_offset + 1][x_offset + 1] == 0)
                    {
                        continue;
                    }

                    if (playing_field[this->cord_y + y_offset + 1][this->cord_x + x_offset].is_block)
                    {
                        return 1;
                    }

                    break;
                }
            }

            break;

        case (M_LEFT):

            if (this->cord_x == 0) {return 1;}

            for (int8_t y_offset = -1; y_offset < 2; y_offset++)
            {
                if (this->shape[y_offset + 1][0] == 1 && this->cord_x == 1)
                {
                    return 1;
                }

                for (int8_t x_offset = -1; x_offset < 2; x_offset++)
                {
                    if (this->shape[y_offset + 1][x_offset + 1] == 0)
                    {
                        continue;
                    }

                    if (playing_field[this->cord_y + y_offset][this->cord_x + x_offset - 1].is_block)
                    {
                        return 1;
                    }

                    break;
                }
            }

            break;

        case (M_RIGHT):

            if (this->cord_x == 11) {return 1;}

            for (int8_t y_offset = -1; y_offset < 2; y_offset++)
            {
                if (this->shape[y_offset + 1][2] == 1 && this->cord_x == 10)
                {
                    return 1;
                }

                for (int8_t x_offset = 1; x_offset > -2; x_offset--)
                {
                    if (this->shape[y_offset + 1][x_offset + 1] == 0)
                    {
                        continue;
                    }

                    if (playing_field[this->cord_y + y_offset][this->cord_x + x_offset + 1].is_block)
                    {
                        return 1;
                    }

                    break;
                }
            }

            break;
    }

    return 0;
}

/*
Moves the piece in the direction assigned to this.move_direction
Parameters:
this - The current active piece
*/
static void swap_blocks(active_piece_t *this)
{
    assert(this);

    int8_t x_direction = 0; 
    int8_t y_direction = this->move_direction == M_DOWN ? 1 : 0;

    switch (this->move_direction)
    {
        case(M_NONE): case(M_DOWN):
            break;

        case(M_LEFT):
            x_direction = -1;
            break;

        case(M_RIGHT):
            x_direction = 1;
            break;
    }

    for (int8_t y_offset = -1; y_offset < 2; y_offset++)
    {
        for (int8_t x_offset = -1; x_offset < 2; x_offset++)
        {
            if (this->shape[y_offset + 1][x_offset + 1] != 1) {continue;}

            playing_field[this->cord_y + y_offset][this->cord_x + x_offset].is_block = false;
            playing_field[this->cord_y + y_offset][this->cord_x + x_offset].color = set_color(0, 0, 0, 0);
        }
    }

    this->cord_x += x_direction;
    this->cord_y += y_direction;
    
    for (int8_t y_offset = -1; y_offset < 2; y_offset++)
    {
        for (int8_t x_offset = -1; x_offset < 2; x_offset++)
        {
            if (this->shape[y_offset + 1][x_offset + 1] != 1) {continue;}

            playing_field[this->cord_y + y_offset][this->cord_x + x_offset].is_block = true;
            playing_field[this->cord_y + y_offset][this->cord_x + x_offset].color = shapes_color[active_piece.shape_id]; 

        }
    }
}

/*
Calls the check_move and swap_blocks to move the active piece in the specified direction if possible.
Returns a 1 if the active piece failed to move down.
Parameters:
this - The current active piece
direction - The direction in which the piece will attempt to move. Use the enumerated values defined in game.h (example M_LEFT).
*/
static uint8_t move_piece(active_piece_t *this, uint8_t direction)
{
    assert(this);

    this->move_direction = direction;

    if (this->move_direction == M_NONE)
    {
        write_log("Active piece move function called with argument NONE", LOG_OUT_FILE | LOG_TYPE_WRN);
        return 0;
    }

    if (check_move(this))
    {
        if (this->move_direction == M_DOWN)
        {
            return 1;
        }
    }
    else
    {
        swap_blocks(this);
    }

    return 0;
}

/*
Checks if the requested rotation is valid. Returns a 1 if the rotation is not allowed
Parameters:
this - The current active piece
*/
static uint8_t check_rotation(active_piece_t *this)
{
    assert(this);

    if (this->cord_x == 0 || this->cord_x == 11 || this->cord_y == 21)
    {
        return 1;
    }

    uint8_t rotated_shape[3][3];

    switch (this->rotate_direction)
    {
        case (R_NONE):
            
            return 1;

        case (R_CLOCKWISE):

            for (uint8_t i = 0; i < 3; i++)
            {
                for (uint8_t j = 0; j < 3; j++)
                {
                    rotated_shape[i][j] = this->shape[j][i];
                }
            }

            for (uint8_t i = 0; i < 3; i++)
            {
                swap(&rotated_shape[i][0], &rotated_shape[i][2], 1);
            }

            break;

        case (R_CONTCLOCKWISE):

            for (int8_t i = 2; i > -1; i--)
            {
                for (int8_t j = 0; j < 3; j++)
                {
                    rotated_shape[i][j] = this->shape[j][i];
                }
            }

            for (uint8_t i = 0; i < 3; i++)
            {
                swap(&rotated_shape[0][i], &rotated_shape[2][i], 1);
            }

            break;
    }

    for (int8_t y_offset = -1; y_offset < 2; y_offset++)
    {
        for (int8_t x_offset = -1; x_offset < 2; x_offset++)
        {
            if (!rotated_shape[y_offset + 1][x_offset + 1]) {continue;}

            if (this->cord_x + x_offset < 0 || this->cord_x + x_offset > 11 || this->cord_y + y_offset > 21)
            {
                return 1;
            }
            else if (this->shape[y_offset + 1][x_offset + 1] == 0 
            && playing_field[this->cord_y + y_offset][this->cord_x + x_offset].is_block)
            {
                return 1;
            }
        }
    }

    return 0;
}

/*
Rotates the piece in the direction assigned to this.rotate_direction
Parameters:
this - The current active piece
*/
static void rotate_blocks(active_piece_t *this)
{
    assert(this);

    uint8_t rotated_shape[3][3];

    switch (this->rotate_direction)
    {
        case (R_NONE):

            return;

        case (R_CLOCKWISE):

            for (uint8_t i = 0; i < 3; i++)
            {
                for (uint8_t j = 0; j < 3; j++)
                {
                    rotated_shape[i][j] = this->shape[j][i];
                }
            }

            for (uint8_t i = 0; i < 3; i++)
            {
                swap(&rotated_shape[i][0], &rotated_shape[i][2], 1);
            }

            break;

        case (R_CONTCLOCKWISE):

            for (int8_t i = 2; i > -1; i--)
            {
                for (int8_t j = 0; j < 3; j++)
                {
                    rotated_shape[i][j] = this->shape[j][i];
                }
            }

            for (uint8_t i = 0; i < 3; i++)
            {
                swap(&rotated_shape[0][i], &rotated_shape[2][i], 1);
            }

            break;
    }

    for (int8_t y_offset = -1; y_offset < 2; y_offset++)
    {
        for (int8_t x_offset = -1; x_offset < 2; x_offset++)
        {
            if (this->shape[y_offset + 1][x_offset + 1] != 1) {continue;}

            playing_field[this->cord_y + y_offset][this->cord_x + x_offset].is_block = false;
            playing_field[this->cord_y + y_offset][this->cord_x + x_offset].color = set_color(0, 0, 0, 0);
        }
    }

    memcpy(this->shape, rotated_shape, 9 * sizeof(uint8_t));

    for (int8_t y_offset = -1; y_offset < 2; y_offset++)
    {
        for (int8_t x_offset = -1; x_offset < 2; x_offset++)
        {
            if (this->shape[y_offset + 1][x_offset + 1] != 1) {continue;}

            playing_field[this->cord_y + y_offset][this->cord_x + x_offset].is_block = true;
            playing_field[this->cord_y + y_offset][this->cord_x + x_offset].color = shapes_color[active_piece.shape_id]; 

        }
    }
}

/*
Calls the check_rotation and rotate_blocks to rotate the active piece in the specified direction if possible.
Parameters:
this - The current active piece
direction - The direction in which the piece will attempt to rotate. Use the enumerated values defined in game.h (example R_CLOCKWISE).
*/
static void rotate_piece(active_piece_t *this, uint8_t direction)
{
    assert(this);

    this->rotate_direction = direction;

    if (this->rotate_direction == R_NONE)
    {
        write_log("Active piece rotate function called with argument NONE", LOG_OUT_FILE | LOG_TYPE_WRN);
        return;
    }

    if (!check_rotation(this))
    {
        rotate_blocks(this);
    }

    return;
}

/*
Initializes every piece in the piece_queue and randomly assigns it a shape. Gets called at the start of
the game and every time a new piece is spawned
*/
static void populate_piece_queue()
{
    for (uint8_t i = 0; i < QUEUE_LIMIT; i++)
    {
        if (piece_queue[i] != NULL) {continue;}

        piece_queue[i] = malloc(sizeof(active_piece_t));
        if (!piece_queue[i])
        {
            write_log("Piece queue memory allocation failed", LOG_OUT_BOTH | LOG_TYPE_ERR);
            exit(1);
        }
        piece_queue[i]->cord_x = piece_queue[i]->cord_y = 0;

        piece_queue[i]->shape_id = rand() % PIECE_SHAPES;
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
    for (uint8_t i = 0; i < QUEUE_LIMIT - 1; i++)
    {
        memcpy(piece_queue[i], piece_queue[i + 1], sizeof(active_piece_t));
    }
    free(piece_queue[QUEUE_LIMIT - 1]);
    piece_queue[QUEUE_LIMIT - 1] = NULL;

    populate_piece_queue();

    active_piece.cord_x = 5; active_piece.cord_y = 2;
    active_piece.move_direction = M_NONE;
    active_piece.move = move_piece;
    active_piece.rotate = rotate_piece;

    for (int y_offset = 0; y_offset < 3; y_offset++)
    {
        for (int x_offset = 0; x_offset < 3; x_offset++)
        {
            if (!active_piece.shape[y_offset][x_offset]) {continue;}

            playing_field[2 + (y_offset - 1)][5 + (x_offset - 1)].color = shapes_color[active_piece.shape_id];
            playing_field[2 + (y_offset - 1)][5 + (x_offset - 1)].is_block = true;
        }
    }
}

/*
Unloads the piece queue at the end of the game
*/
static void unload_piece_queue()
{
    for (uint8_t i = 0; i < QUEUE_LIMIT; i++)
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
        check_lines();
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

    static_assert(QUEUE_LIMIT < 9, "Piece limit should be lower than 9");
    static_assert(PIECE_SHAPES == 7, "Change if you want to add more shapes. You have to define the shape and color in the respective arrays.");

    write_log("Starting game...", LOG_TYPE_INF | LOG_OUT_FILE);

    for (int i = 0; i < 22; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            playing_field[i][j].is_block = false;
            playing_field[i][j].color = set_color(0, 0, 0, 0);
        }
    }

    init_shape_color();
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