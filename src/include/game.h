#pragma once

typedef enum
{
    M_NONE,
    M_LEFT,
    M_RIGHT,
    M_DOWN
}
move_direction_t;

typedef enum
{
    R_NONE,
    R_LEFT,
    R_RIGHT
}
rotate_direction_t;

typedef struct
{
    bool is_block;
    bool is_static;
    Rgba_t color;
}
Block_t;

typedef struct active_piece_t
{
    uint8_t cord_x, cord_y;
    uint8_t shape[3][3];
    uint8_t shape_id;
    union
    {
        move_direction_t move_direction;
        rotate_direction_t rotate_direction;
    };
    uint8_t (*move)(struct active_piece_t *, uint8_t);
}
active_piece_t;

extern Block_t playing_field[22][12];
extern active_piece_t active_piece;

/*
(UNFINISHED) Increments the game clock and handles game logic. Should be called every frame.
*/
extern void tick_logic(void);

/*
(UNFINISHED) Initializes game logic variables.
*/
extern void start_game(void);

/*

*/
extern void unload_game(void);