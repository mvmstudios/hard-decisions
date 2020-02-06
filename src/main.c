#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "game.h"

int window_width = 800;
int window_height = 600;

int mouse_x;
int mouse_y;

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_Window* window = SDL_CreateWindow("Help plis :C", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Init();

    game_t* game = game_create(renderer);

    const Uint8* const keyboard_state = SDL_GetKeyboardState(NULL);

    uint64_t performance_frequency = SDL_GetPerformanceFrequency();
    uint64_t performance_counter = SDL_GetPerformanceCounter();

    float delta_time = 0;
    double global_time = 0;

    bool close_requested = false;
    while (!close_requested) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT)
                close_requested = true;
            if (event.type == SDL_KEYDOWN)
                game_input_key_down(game, event.key.keysym.scancode);
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window_width = event.window.data1;
                    window_height = event.window.data2;
                }
            }

            game_event(game, &event);
            
        }

        SDL_GetMouseState(&mouse_x, &mouse_y);

        game_keyboard_state(game, keyboard_state);
        game_update(game, delta_time, global_time);
        game_render(game);

        global_time += delta_time;
        uint64_t performance_counter_at_end = SDL_GetPerformanceCounter();
        uint64_t elapsed = performance_counter_at_end - performance_counter;

        delta_time = (float) elapsed / (float) performance_frequency;

        performance_counter = performance_counter_at_end;
    } 
    
}