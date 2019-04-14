#include <stdlib.h>

#include "game.h"
#include "text_renderer.h"

extern int mouse_x;
extern int mouse_y;

typedef struct {
    game_t* game;
    text_renderer_t* text_renderer;
    
    void (*click_action_func)(button_t*);
    char* text;

    int x;
    int y;
    int width;
    int height;
} button_t;

button_t* button_create(game_t* game, void (*click_action_func)(button_t*), char* text, int x, int y, int width, int height) {
    button_t* button = malloc(sizeof(button_t));

    button->game = game;
    
    button->click_action_func = click_action_func;
    button->text = text;
    
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;

    return button;
}

bool button_mouse_over(button_t* button) {
    return  (mouse_x > button->x && mouse_x < button->x + button->width) && (mouse_y > button->y && mouse_y < button->y + button->height);
}

void button_event(button_t* button, SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONDOWN)
        if (event->button.button == SDL_BUTTON_RIGHT)
            if (button_mouse_over(button))
                (*button->click_action_func)(button);
                    
} 

void button_render(button_t* button) {
    SDL_Rect rect = { button->x, button->y, button->width, button->height };
    SDL_RenderDrawRect(button->game->renderer, &rect);
}