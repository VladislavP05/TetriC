// util.c

// Defines global functions in the program

#include "../include/util.h"
#include "../include/var.h"

// Returns an RGBA_t value from the specified color parameters
extern RGBA_t set_color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
{
    RGBA_t color = {.red = red, .green = green, .blue = blue, .alpha = alpha};

    return color;
}

// Initializes the playing field
extern void init_playing_field(void)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            playing_field[i][j].is_block = false;
            playing_field[i][j].color = set_color(0, 0, 0, 0);
        }
    }

    playing_field[0][2].is_block = true;
    playing_field[0][2].color = set_color(255, 255, 255, 255);
}