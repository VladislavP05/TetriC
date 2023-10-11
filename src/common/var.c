// var.c

// Defines global variables in the program

#include "../include/var.h"

bool game_closed = false;

GAME_t game = {.renderer=NULL, .window=NULL};

BLOCK_t playing_field[20][10];

float delta_time = 0.0;