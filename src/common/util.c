// util.c

// Defines global functions in the program

#include "util.h"
#include "var.h"
#include "log.h"

extern void swap(void *var1, void *var2, size_t size)
{
    assert(var1 && var2 && size);

    void *tmp = malloc(size);
    if (!tmp)
    {
        write_log("Swap memory allocation failed", LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    memmove(tmp, var1, size);
    memmove(var1, var2, size);
    memmove(var2, tmp, size);

    free(tmp);

    return;
}

extern SDL_Color set_color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
{
    SDL_Color color = {.r = red, .g = green, .b = blue, .a = alpha};
    return color;
}
