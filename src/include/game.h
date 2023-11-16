#pragma once

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
    enum move_direction
    {
        NONE,
        DOWN,
        LEFT,
        RIGHT
    } curr_move_direcrion;
    void (*move)(struct active_piece_t *, const uint8_t);
    struct active_piece_t *next_piece;
    
}active_piece_t;

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