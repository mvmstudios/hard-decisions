#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "game.h"

int main(int argc, char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    game_t* game = game_create(renderer);

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
        }

        game_update(game, delta_time, global_time);
        game_render(game);
    } 
    
}