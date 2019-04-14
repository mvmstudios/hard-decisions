#include <stdlib.h>
#include <stdbool.h>

#include "text_renderer.h"

extern int mouse_x;
extern int mouse_y;

typedef struct {
    SDL_Renderer* sdl_renderer;
    text_renderer_t* text_renderer;
    
    char* text;

    int x;
    int y;
    int width;
    int height;
} button_t;

button_t* button_create(SDL_Renderer* sdl_renderer, char* text, int x, int y, int width, int height) {
    button_t* button = malloc(sizeof(button_t));

    button->sdl_renderer = sdl_renderer;
    button->text_renderer = text_renderer_create(sdl_renderer, "resources/fonts/carbon bl.ttf", 16, SDL_WHITE_COLOR);
    
    button->text = text;
    text_renderer_set_text(button->text_renderer, text);
    
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;

    return button;
}

bool button_mouse_over(button_t* button) {
    return  (mouse_x > button->x && mouse_x < button->x + button->width) && (mouse_y > button->y && mouse_y < button->y + button->height);
}

void button_render(button_t* button) {
    if (button_mouse_over(button))
        SDL_SetRenderDrawColor(button->sdl_renderer, 30, 255, 30, 255);
    else
        SDL_SetRenderDrawColor(button->sdl_renderer, 0, 0, 0, 255);

    SDL_Rect rect = { button->x, button->y, button->width, button->height };
    SDL_RenderFillRect(button->sdl_renderer, &rect);

    text_renderer_render(button->text_renderer, rect);
}