// util.c

// Defines global functions in the program

#include "../include/util.h"
#include "../include/var.h"
#include "../include/log.h"

extern void swap(void *var1, void *var2, size_t size)
{
    assert(var1 && var2);

    void *tmp = malloc(size);
    if (!tmp) {write_log("Memory for swap not allocated", LOG_OUT_BOTH | LOG_TYPE_ERR); exit(1);}

    memmove(tmp, var1, size);
    memmove(var1, var2, size);
    memmove(var2, tmp, size);

    free(tmp);
}

extern RGBA_t set_color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
{
    RGBA_t color = {.red = red, .green = green, .blue = blue, .alpha = alpha};

    return color;
}

extern void init_playing_field(void)
{
    for (int i = 0; i < 22; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            playing_field[i][j].is_block = false;
            playing_field[i][j].color = set_color(0, 0, 0, 0);
        }
    }


    // For Testing Purposes
    playing_field[0][4].is_block = true;
    playing_field[0][5].is_block = true;
    playing_field[0][6].is_block = true;
    playing_field[0][4].color = set_color(255, 255, 255, 255);
    playing_field[0][5].color = set_color(255, 255, 255, 255);
    playing_field[0][6].color = set_color(255, 255, 255, 255);
}