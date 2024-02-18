#pragma once

typedef struct
{
    uint8_t id;
    SDL_Texture *texture;
    SDL_Rect rect;
    SDL_Surface *surface;
}
ui_element_t;

extern SDL_Surface *queue_textures[];

extern ui_element_t create_text_box(const char *message, SDL_Color color, uint16_t win_x, uint16_t win_y, float scale);

extern void refresh_text_box(ui_element_t *text_box, const char *message, SDL_Color color);

extern ui_element_t create_queue_box(uint8_t shape_id, uint16_t win_x, uint16_t win_y);

extern void destroy_text_box(ui_element_t text_box);

extern void unload_ui(void);

extern void init_ui(void);