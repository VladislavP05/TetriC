#include <math.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "ui.h"
#include "var.h"
#include "log.h"
#include "game.h"

#define FONT_SIZE 100   // Size of the font used by the ui

SDL_Texture *queue_textures[PIECE_SHAPES];  // Array containing the textures for the queue

static TTF_Font *ui_font;   // The font of the ui

/*
Unloads the textures in queue_textures
*/
static void unload_queue_textures(void)
{
    for (uint8_t i = 0; i < PIECE_SHAPES; i++)
    {
        SDL_DestroyTexture(queue_textures[i]);
    }

    return;
}

/*
Initializes a ui_element_t struct for use in the queue.
Parameters:
shape_id - The id of the shape in the queue
win_x - The x coordinate of the box
win_y - The y coordinate of the box
*/
extern ui_element_t create_queue_box(uint8_t shape_id, uint16_t win_x, uint16_t win_y)
{
    ui_element_t queue_box;

    queue_box.texture = queue_textures[shape_id];

    if (!queue_box.texture)
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    queue_box.rect.h = queue_box.rect.w = 50;
    queue_box.rect.x = win_x;
    queue_box.rect.y = win_y;

    return queue_box;
}

/*
Loads the texture files into the queue_textures array.
*/
extern SDL_Texture * load_texture(const char *file)
{
    SDL_Surface *tmp;
    SDL_Texture *texture_out;
    char full_path[50] = "Textures/\0";

    strcat(full_path, file);
    tmp = SDL_LoadBMP(full_path);

    texture_out = SDL_CreateTextureFromSurface(game.renderer, tmp);

    if (!texture_out)
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    SDL_FreeSurface(tmp);

    return texture_out;
}

/*
Initializes a ui_element_t struct for displaying text.
Parameters:
message - The message to be displayed
color - The color of the message
win_x - The x coordinate of the box
win_y - The y coordinate of the box
text_size - The size of the text
*/
extern ui_element_t create_text_box(const char *message, SDL_Color color, uint16_t win_x, uint16_t win_y, float scale)
{
    ui_element_t text_box;
    static uint8_t ui_id = 0;

    text_box.surface = TTF_RenderText_Solid(ui_font, message, color);

    if (!text_box.surface)
    {
        write_log(TTF_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    text_box.texture = SDL_CreateTextureFromSurface(game.renderer, text_box.surface);

    if (!text_box.texture)
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    int ln_coef = strlen(message);//(int)sqrt((float)strlen(message));    // Cursed

    text_box.rect.x = win_x;
    text_box.rect.y = win_y;
    text_box.rect.h = FONT_SIZE * scale;
    text_box.rect.w = (ln_coef * FONT_SIZE / 2) * scale;

    text_box.id = ui_id;

    ui_id++;

    return text_box;
}

extern void refresh_text_box(ui_element_t *text_box, const char *message, uint16_t cord_x, uint16_t cord_y, SDL_Color color)
{
    assert(text_box);

    SDL_FreeSurface(text_box->surface);
    SDL_DestroyTexture(text_box->texture);

    text_box->surface = TTF_RenderText_Solid(ui_font, message, color);
    
    if (!text_box->surface)
    {
        write_log(TTF_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    text_box->texture = SDL_CreateTextureFromSurface(game.renderer, text_box->surface);

    if (!text_box->texture)
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    text_box->rect.x = cord_x;
    text_box->rect.y = cord_y;

    return;
}

/*
Frees memory used by the text box.
Parameters:
text_box - The text box to be freed
*/
extern void destroy_text_box(ui_element_t text_box)
{
    SDL_DestroyTexture(text_box.texture);
    SDL_FreeSurface(text_box.surface);
}

/*
Unloads resources used by the ui.
*/
extern void unload_ui(void)
{
    unload_queue_textures();
    TTF_CloseFont(ui_font);
    TTF_Quit();
    IMG_Quit();
}

/*
Initializes the ui.
*/
extern void init_ui(void)
{
    static_assert(FONT_SIZE < INT32_MAX, "Font size has to be within INT value limits");

    IMG_Init(IMG_INIT_PNG);
    
    if (TTF_Init())
    {
        write_log("TTF failed to initialize", LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    ui_font = TTF_OpenFont("Textures/font.ttf", FONT_SIZE);

    if (!ui_font)
    {
        write_log(TTF_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    write_log("TTF initialized", LOG_OUT_FILE | LOG_TYPE_INF);

    for (uint8_t i = 0; i < PIECE_SHAPES; i++)
    {
        char file[] = "Shape?.bmp";
        file[5] = (char) i + 49;

        queue_textures[i] = load_texture(file);
    }
    
}