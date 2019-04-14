#include <stdlib.h>

#include <SDL2/SDL_ttf.h>

const SDL_Color SDL_WHITE_COLOR = { 255, 255, 255 };

typedef struct {
    char* text;

    SDL_Renderer* sdl_renderer;

    SDL_Color font_color;
    TTF_Font* font;
    SDL_Texture* texture;
} text_renderer_t;

void text_renderer_set_text(text_renderer_t* renderer, char* text) {
    renderer->text = text;

    SDL_Surface* surface = TTF_RenderText_Solid(renderer->font, renderer->text, renderer->font_color);
    renderer->texture = SDL_CreateTextureFromSurface(renderer->sdl_renderer, surface);
    SDL_FreeSurface(surface);
}

void text_renderer_update_font(text_renderer_t* renderer, char* path_to_font, int font_size) {
    renderer->font = TTF_OpenFont(path_to_font, font_size);
    if (!renderer->font)
        printf("Error loading font!\n");
    text_renderer_set_text(renderer, renderer->text);
}

void text_renderer_update_font_color(text_renderer_t* renderer, SDL_Color font_color) {
    renderer->font_color = font_color;
    text_renderer_set_text(renderer, renderer->text);
}

void text_renderer_render(text_renderer_t* renderer, SDL_Rect dest) {
    SDL_RenderCopy(renderer->sdl_renderer, renderer->texture, NULL, &dest);
}

text_renderer_t* text_renderer_create(SDL_Renderer* sdl_renderer, char* path_to_font, int font_size, SDL_Color font_color) {
    text_renderer_t* renderer = malloc(sizeof(text_renderer_t));

    renderer->sdl_renderer = sdl_renderer;

    renderer->text = "NULL";
    renderer->font_color = SDL_WHITE_COLOR;

    text_renderer_update_font(renderer, path_to_font, font_size);
    
    return renderer;
}