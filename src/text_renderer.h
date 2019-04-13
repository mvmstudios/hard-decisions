#include <SDL2/SDL_ttf.h>

static const SDL_Color SDL_WHITE_COLOR = { 255, 255, 255 };

typedef struct {
    char* current_text;

    SDL_Color font_color;
    TTF_Font* font;
    SDL_Texture* texture;
} text_renderer_t;

text_renderer_t* renderer_create(const char* path_to_font, int font_size = 16, SDL_Color font_color = SDL_WHITE_COLOR) {
    text_renderer_t renderer = malloc(sizeof(text_renderer_t));

    text_renderer_set_text(renderer, "NULL");

    return renderer;
}

void text_renderer_update_font(text_renderer_t* renderer, const char* path_to_font, int font_size = 16) {
    renderer->font = TTF_OpenFont(path_to_font, font_size);
    text_renderer_set_text(renderer, renderer->text);
}

void text_renderer_update_font_color(text_renderer_t* renderer, SDL_Color font_color) {
    renderer->font_color = font_color;
    text_renderer_set_text(renderer->text);
}

void text_renderer_set_text(text_renderer_t* renderer, const char* text) {
    renderer->text = text;

    SDL_Surface* surface = TTF_RenderText_Solid(renderer->font, text, renderer->font_color);
    renderer->texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
    SDL_FreeSurface(surface);
}

void text_renderer_render(text_renderer_t* renderer, SDL_Rect dest, SDL_Renderer* sdl_renderer) {
    SDL_RenderCopy(sdl_renderer, renderer->texture, NULL, &dest);
}