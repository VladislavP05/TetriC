// output.c

// Rendering of the program.

#include "include/var.h"
#include "include/util.h"
#include "include/output.h"
#include "include/log.h"
#include "include/game.h"
#include "include/ui.h"

#define PLAYING_FIELD_WIDTH 300                                      // The playing field width in pixels
#define PLAYING_FIELD_HEIGHT 500                                     // The playing field height in pixels
#define PLAYING_FIELD_X ((WINDOW_WIDTH - PLAYING_FIELD_WIDTH) / 2)   // The playing field x screen coordinate
#define PLAYING_FIELD_Y ((WINDOW_HEIGHT - PLAYING_FIELD_HEIGHT) / 2) // The playing field y screen coordinate
#define BLOCK_SIZE (PLAYING_FIELD_HEIGHT / 20)                       // Size of playing field blocks
#define BLOCK_PADDING 2                                              // The size of padding beween blocks

typedef struct ui_element_node_t
{
    ui_element_t text_box;
    struct ui_element_node_t *next;
}
ui_element_node_t;

static ui_element_node_t *text_elements = NULL;   // The first node in a single linked list used for storing text elements
static ui_element_t *queue_elements[QUEUE_LIMIT]; // Array containing the graphical info about the piece queue
static uint8_t score_text_id;

// TEMP
static const SDL_Color WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
static const SDL_Color BLACK = {.r = 0, .g = 0, .b = 0, .a = 255};

/*
Sets the rendering color to the provided color struct.
Parameters:
color - The color which the render will switch to
*/
static int8_t set_render_col(const SDL_Color color)
{
    return SDL_SetRenderDrawColor(game.renderer, color.r, color.g, color.b, color.a);
}

/*
Adds a text element to the list.
Parameters:
element_pointer - The pointer pointing to the first element in the list
message - The text of the text_box
color - The color of the text
win_x - The x coordinate of the text box
win_y - The y coordinate of the text box
text_size - The size of the text box
*/
static uint8_t add_text_element(ui_element_node_t *element_pointer, const char *message, SDL_Color color, uint16_t win_x, uint16_t win_y, float scale)
{
    assert(element_pointer);

    static uint8_t ui_id;

    if (!element_pointer->next)
    {
        element_pointer->next = calloc(1, sizeof(ui_element_node_t));
        element_pointer->next->text_box = create_text_box(message, color, win_x, win_y, scale);
        ui_id = element_pointer->next->text_box.id;
        return 0;
    }

    add_text_element(element_pointer->next, message, color, win_x, win_y, scale);
    
    return ui_id;
}

static void update_score_text(void)
{
    ui_element_node_t *element_pointer = text_elements;

    while (element_pointer->text_box.id != score_text_id)
    {
        if (!element_pointer->next)
        {
            write_log("Score text box id not found", LOG_OUT_BOTH | LOG_TYPE_ERR);
            exit(1);
        }

        element_pointer = element_pointer->next;
    }

    char score_composite[11];

    for (uint8_t i = 0; i < 10; i++)
    {
        score_composite[i] = 48;
    }

    score_composite[10] = 0;

    char score_str[10];

    uint8_t score_len = snprintf(NULL, 0, "%u", score);

    sprintf(score_str, "%u", score);

    for (uint8_t i = 0; i < score_len; i++)
    {
        score_composite[9 - i] = score_str[(score_len - 1) - i];
    }

    refresh_text_box(&element_pointer->text_box, score_composite, WHITE);

    return;
}

/*
Updates the queue array. Called every frame.
*/
static void update_queue_list(void)
{
    for (uint8_t i = 0; i < QUEUE_LIMIT; i++)
    {
        if (queue_elements[i])
        {
            SDL_DestroyTexture(queue_elements[i]->texture);
            free(queue_elements[i]);
        }
        queue_elements[i] = calloc(1, sizeof(ui_element_t));

        if (!queue_elements[i])
        {
            write_log("Queue Elements allocation failed", LOG_OUT_BOTH | LOG_TYPE_ERR);
            exit(1);
        }

        *queue_elements[i] = create_queue_box(piece_queue[i]->shape_id, PLAYING_FIELD_X + PLAYING_FIELD_WIDTH + 10, PLAYING_FIELD_Y + 50 * (i + 1));
    }

    return;
}

/*
Renders the block array in its current state.
Parameters:
field_x - The x coordinates of the field
field_y - The y coordinates of the field
*/
static void render_block_array(const uint16_t field_x, const uint16_t field_y)
{
    assert(field_x <= WINDOW_WIDTH && field_y <= WINDOW_HEIGHT);

    for (int i = 2; i < 22; i++)
    {
        for (int j = 0; j < 12; j++)
        {
            if (!playing_field[i][j].is_block)
            {
                continue;
            }

            set_render_col(playing_field[i][j].color);

            SDL_Rect block_rect = 
            {
            .h = BLOCK_SIZE - BLOCK_PADDING,
            .w = BLOCK_SIZE - BLOCK_PADDING,
            .x = field_x + (j * BLOCK_SIZE),
            .y = field_y + i * BLOCK_SIZE - BLOCK_SIZE * 2
            };

            SDL_RenderFillRect(game.renderer, &block_rect);

            SDL_RenderDrawRect(game.renderer, &block_rect);
        }
    }

    return;
}

/*
Renders the playing field.
Parameters:
field_x - The x coordinates of the field
field_y - The y coordinates of the field
*/
static void render_playing_field(uint16_t pos_x, uint16_t pos_y)
{
    assert(pos_x <= WINDOW_WIDTH && pos_y <= WINDOW_HEIGHT);

    const uint16_t offset_y = 50;

    SDL_Rect rect = {.h = PLAYING_FIELD_HEIGHT, .w = PLAYING_FIELD_WIDTH, .x = pos_x, .y = pos_y + offset_y};

    render_block_array(pos_x, pos_y + offset_y);

    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);

    SDL_RenderDrawRect(game.renderer, &rect);

    return;
}

/*
Renders the ui elements contained in text_elements and queue_elements.
*/
static void render_ui(void)
{
    ui_element_node_t *element_pointer = text_elements;

    update_score_text();

    while (element_pointer)
    {
        SDL_RenderCopy(game.renderer, element_pointer->text_box.texture, NULL, &element_pointer->text_box.rect);
        element_pointer = element_pointer->next;
    }
    
    for (uint8_t i = 0; i < QUEUE_LIMIT; i++)
    {
        SDL_RenderCopy(game.renderer, queue_elements[i]->texture, NULL, &queue_elements[i]->rect);
    }

    return;
}

/*
Frees the memory used by the element list
Parameters:
element_pointer - The pointer pointing to the first element in the list
*/
static void free_element_list(ui_element_node_t *element_pointer)
{
    assert(element_pointer);

    if (!element_pointer->next)
    {
        destroy_text_box(element_pointer->text_box);
        free(element_pointer);
        return;
    }

    free_element_list(element_pointer->next);
    destroy_text_box(element_pointer->text_box);
    free(element_pointer);
    return;
}

/*
Calls the separate render functions and presents the scene.
*/
extern void render_frame(void)
{
    if (SDL_RenderClear(game.renderer))
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    render_playing_field(PLAYING_FIELD_X, PLAYING_FIELD_Y);

    update_queue_list();

    render_ui();

#ifdef UI_GUIDES

    set_render_col(WHITE);
    SDL_RenderDrawLine(game.renderer, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);
    SDL_RenderDrawLine(game.renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2);

#endif

    set_render_col(BLACK);

    SDL_RenderPresent(game.renderer);

    return;
}

/*
Initializes the output systems of the program.
*/
extern void init_output(void)
{
    game.window = SDL_CreateWindow("TetriC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!game.window)
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

    if (!game.renderer)
    {
        write_log(SDL_GetError(), LOG_OUT_BOTH | LOG_TYPE_ERR);
        exit(1);
    }

    init_ui();

    write_log("Game renderer and window created", LOG_OUT_FILE | LOG_TYPE_INF);

    text_elements = calloc(1, sizeof(ui_element_node_t));

    add_text_element(text_elements, "Score", WHITE, WINDOW_WIDTH / 3 + 34, 55, 0.45);
    score_text_id = add_text_element(text_elements, "000000000", WHITE, WINDOW_HEIGHT / 4 - 22, 100, 0.5);

    return;
}

/*
Unloads resources used by the output system.
*/
extern void unload_output(void)
{
    free_element_list(text_elements);

    unload_ui();

    SDL_DestroyRenderer(game.renderer);

    SDL_DestroyWindow(game.window);

    write_log("Game window and renderer destroyed", LOG_OUT_FILE | LOG_TYPE_INF);

    return;
}