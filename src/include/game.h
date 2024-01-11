#pragma once

#define FPS 75

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
    R_CLOCKWISE,
    R_CONTCLOCKWISE
}
rotate_direction_t;

typedef struct
{
    bool is_block;
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
    void (*rotate)(struct active_piece_t *, uint8_t);
}
active_piece_t;

extern Block_t playing_field[22][12];
extern active_piece_t active_piece;

extern void tick_logic(void);

extern void start_game(void);

extern void unload_game(void);