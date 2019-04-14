#include <stdlib.h>
#include <stdio.h>
#include <time.h> 

#include <SDL2/SDL.h>

#include "text_renderer.h"

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

typedef struct {
    SDL_Renderer* renderer;
    text_renderer_t* text_renderer;

    color_t* current_background_color;
    color_t* current_displayed_color;

    guess_t current_guess;

    float time_since_last_change;
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

    text_renderer_set_text(game->text_renderer, game->current_displayed_color->name);

    game->current_guess = GUESS_NONE;

    game->time_since_last_change = 0;
}

game_t* game_create(SDL_Renderer* renderer) {
    game_t* game = malloc(sizeof(game_t));

    game->renderer = renderer;
    game->text_renderer = text_renderer_create(renderer, "resources/fonts/carbon bl.ttf", 16, SDL_WHITE_COLOR);
    printf("created text_renderer YAY!\n");

    game_next_color(game);

    return game;
}

void game_lose(game_t* game) {
    exit(-1);
}

void game_update(game_t* game, float delta_time, double global_time) {
    game->time_since_last_change += delta_time;
    printf("%f\n", game->time_since_last_change);
    
    if (game->time_since_last_change >= 5 && game->current_guess == GUESS_NONE) {
        game_lose(game);
    }

    if (game->current_guess != GUESS_NONE) {
        if (game->current_guess == GUESS_RIGHT) {
            if (colors_equal(game->current_background_color, game->current_displayed_color))
                game_next_color(game);
            else
                game_lose(game);
        }

        if (game->current_guess == GUESS_WRONG) {
            if (colors_equal(game->current_background_color, game->current_displayed_color))
                game_lose(game);
            else
                game_next_color(game);
        }
    }
}

void game_input_key_down(game_t* game, SDL_Scancode scancode) {
    printf("HX\n");
    switch (scancode) {
        case SDL_SCANCODE_RIGHT:
            game->current_guess = GUESS_RIGHT;
            break;
        case SDL_SCANCODE_LEFT:
            game->current_guess = GUESS_WRONG;
            break;
    }
}

void game_keyboard_state(game_t* game, const Uint8* const keyboard_state) {
    
}

void game_render(game_t* game) {
    SDL_SetRenderDrawColor(game->renderer, game->current_background_color->r, game->current_background_color->g, game->current_background_color->b, 255);
    SDL_RenderClear(game->renderer);

    text_renderer_render(game->text_renderer, (SDL_Rect) {0, 0, 300, 100});

    SDL_RenderPresent(game->renderer);
}