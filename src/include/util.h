#pragma once

#include "var.h"

/*
Swaps the values of two variables.

Parameters:
    var1 - Variable 1
    var2 - Variable 2
    size - Size of the variables in bytes. The size of the two variables should be equal!
*/
extern void swap(void *var1, void *var2, size_t size);

/*
Returns an RGBA_t struct initialized with the provided arguments. All values should be between 0-255.
*/
extern RGBA_t set_color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha);

/*
(UNFINISHED) Initializes the playing field. (Should be moved to game.c)
*/
extern void init_playing_field(void);