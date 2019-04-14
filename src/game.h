#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h> 

#include <SDL2/SDL.h>

#include "text_renderer.h"
#include "button.h"

extern int window_width;
extern int window_height;

typedef struct {
    int r, g, b;
    char* name;
} color_t;

color_t colors[] = {
    { 255, 0, 0, "Red" },
    { 0, 255, 0, "Green" },
    { 0, 0, 0, "Black" },
    { 139, 69, 19, "Brown" },
    { 255, 20, 147, "Pink" },
    { 125, 38, 205, "Purple" }, 
    { 255, 255, 0, "Yellow" },
    { 37, 189, 177, "Cyan" },
    { 255, 127, 0, "Orange" },
    { 0, 0, 255, "Blue" }
}; 
int colors_size = 10;

unsigned int seed;

color_t* get_random_color() {
    static unsigned int seed;

    srand(seed + rand() + time(0));

    return &colors[rand() % colors_size];
}

bool colors_equal(color_t* color_x, color_t* color_y) {
    return color_x->name == color_y->name;
}

typedef enum {
    GUESS_RIGHT,
    GUESS_WRONG,
    GUESS_NONE
} guess_t;

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_LOST,
    GAME_STATE_INGAME
} game_state_t;

typedef struct {
    SDL_Renderer* renderer;

    text_renderer_t* color_text_renderer;
    text_renderer_t* points_text_renderer;

    game_state_t state;

    color_t* current_background_color;
    color_t* current_displayed_color;

    bool started;

    guess_t current_guess;

    float time_since_last_change;

    int points;

    button_t* menu_start_button;
    button_t* lost_restart_button;
} game_t;

void game_next_color(game_t* game) {
    if (rand() % 100 > 40) {
        color_t* color = get_random_color();
        game->current_background_color = color;
        game->current_displayed_color = color;
    } else {
        game->current_background_color = get_random_color();
        game->current_displayed_color = get_random_color();
    }

    text_renderer_set_text(game->color_text_renderer, game->current_displayed_color->name);

    game->current_guess = GUESS_NONE;

    game->time_since_last_change = 0;

    game->points++;

    char buffer[5];
    sprintf(buffer, "%d", game->points);

    text_renderer_set_text(game->points_text_renderer, buffer);
}

game_t* game_create(SDL_Renderer* renderer) {
    game_t* game = malloc(sizeof(game_t));

    game->renderer = renderer;
   // game->menu = game_menu_create(game);

    game->color_text_renderer = text_renderer_create(renderer, "resources/fonts/carbon bl.ttf", 16, SDL_WHITE_COLOR);
    game->points_text_renderer = text_renderer_create(renderer, "resources/fonts/carbon bl.ttf", 16, SDL_WHITE_COLOR);

    game->state = GAME_STATE_MENU;

    game->started = false;
    game->points = -1;

    game_next_color(game);

    int button_width = window_width / 5;
    int button_height = window_height / 7;

    //// START BUTTON

    game->menu_start_button = button_create(game->renderer, "START", 0, 0, 0, 0);

    game->menu_start_button->x = (window_width / 2) - (button_width / 2);
    game->menu_start_button->y = (window_height / 2) - (button_height / 2);

    game->menu_start_button->width = button_width;
    game->menu_start_button->height = button_height;

    //// RESTART BUTTON

    game->lost_restart_button = button_create(game->renderer, "RESTART", 0, 0, 0, 0);

    game->lost_restart_button->x = (window_width / 2) - (button_width / 2);
    game->lost_restart_button->y = (window_height / 2) - (button_height / 2);

    game->lost_restart_button->width = button_width;
    game->lost_restart_button->height = button_height;

    ////

    return game;
}

void game_lose(game_t* game) {
    char buffer[50];
    sprintf(buffer, "Sorry boii. Your score: %d", game->points);

    text_renderer_set_text(game->points_text_renderer, buffer);
    game->state = GAME_STATE_LOST;
}

void game_update(game_t* game, float delta_time, double global_time) {
    if (game->state == GAME_STATE_INGAME) {
        if (game->started)
            game->time_since_last_change += delta_time;
    // printf("%f\n", game->time_since_last_change);
        
        if (game->time_since_last_change >= 1 && game->current_guess == GUESS_NONE)
            game_lose(game);

        if (game->current_guess != GUESS_NONE) {
            if ((game->current_guess == GUESS_RIGHT && colors_equal(game->current_background_color, game->current_displayed_color)) || (game->current_guess == GUESS_WRONG && !colors_equal(game->current_background_color, game->current_displayed_color)))
                game_next_color(game);
            else
                game_lose(game);
        }
    }

    if (game->state == GAME_STATE_MENU) {
    }

    if (game->state == GAME_STATE_LOST) {

    }
}

void game_restart(game_t* game) {
    game->started = false;
    game->points = -1;
    game_next_color(game);

    game->state = GAME_STATE_INGAME;
}

void game_input_key_down(game_t* game, SDL_Scancode scancode) {
    if (game->state == GAME_STATE_INGAME) {
        switch (scancode) {
            case SDL_SCANCODE_RIGHT:
                game->current_guess = GUESS_RIGHT;
                if (!game->started)
                    game->started = true;
                break;
            case SDL_SCANCODE_LEFT:
                game->current_guess = GUESS_WRONG;
                if (!game->started)
                    game->started = true;
                break;
        }
    }
}

void game_keyboard_state(game_t* game, const Uint8* const keyboard_state) {
    
}

void game_event(game_t* game, SDL_Event* event) {
    if (game->state == GAME_STATE_MENU) {
        if (event->type == SDL_MOUSEBUTTONDOWN) {
            if (event->button.button == SDL_BUTTON_LEFT) {
                if (button_mouse_over(game->menu_start_button)) {
                    game->state = GAME_STATE_INGAME;
                }
            }
        }
    }

    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            if (game->state == GAME_STATE_MENU) {
                if (button_mouse_over(game->menu_start_button)) {
                    game->state = GAME_STATE_INGAME;
                }
            }

            if (game->state == GAME_STATE_LOST) {
                if (button_mouse_over(game->lost_restart_button)) {
                    game_restart(game);
                }
            }
        }
    }

    if (event->type == SDL_WINDOWEVENT) {
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            int button_width = window_width / 5;
            int button_height = window_height / 7;

            //// START BUTTON

            game->menu_start_button->x = (window_width / 2) - (button_width / 2);
            game->menu_start_button->y = (window_height / 2) - (button_height / 2);

            game->menu_start_button->width = button_width;
            game->menu_start_button->height = button_height;

            //// RESTART BUTTON

            game->lost_restart_button->x = (window_width / 2) - (button_width / 2);
            game->lost_restart_button->y = (window_height / 2) - (button_height / 2);

            game->lost_restart_button->width = button_width;
            game->lost_restart_button->height = button_height;

            ////
        }
    }
}

void game_render(game_t* game) {
    if (game->state == GAME_STATE_INGAME) {
        SDL_SetRenderDrawColor(game->renderer, game->current_background_color->r, game->current_background_color->g, game->current_background_color->b, 255);
        SDL_RenderClear(game->renderer);

        text_renderer_render(game->color_text_renderer, (SDL_Rect) { (window_width / 2) - ((window_width / 6)), (window_height / 2) - ((window_height / 8)), window_width / 3, window_height / 4 });
        text_renderer_render(game->points_text_renderer, (SDL_Rect) { (window_width / 2) - ((window_width / 20)), (window_height / 2) - ((window_height / 2)), window_width / 10, window_width / 5 });
    }

    if (game->state == GAME_STATE_MENU) {
        SDL_SetRenderDrawColor(game->renderer, 234, 232, 12, 255);
        SDL_RenderClear(game->renderer);

        button_render(game->menu_start_button);
    }

    if (game->state == GAME_STATE_LOST) {
        SDL_SetRenderDrawColor(game->renderer, 255, 50, 50, 255);
        SDL_RenderClear(game->renderer);

        text_renderer_render(game->points_text_renderer, (SDL_Rect) { (window_width / 2) - ((window_width / 4)), (window_height / 2) - ((window_height / 2)), window_width / 2, window_width / 5 });
        button_render(game->lost_restart_button);
    }

    SDL_RenderPresent(game->renderer);
}