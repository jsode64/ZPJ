#include "menu.h"
#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>

Menu::Menu() {}

void Menu::render_button(const Button& button, SDL_Renderer* renderer, TTF_Font* font) const {
    // Draw button background
    SDL_Color bg_color = SDL_Color{50, 100, 200, 255};
    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderFillRect(renderer, &button.body);

    // Draw button border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &button.body);

    // Draw button text
    if (font) {
        SDL_Surface* text_surface =
            TTF_RenderText_Solid(font, button.label.c_str(), button.label.length(), SDL_Color{255, 255, 255, 255});
        if (text_surface) {
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            if (text_texture) {
                SDL_FRect text_rect{button.body.x + (button.body.w - text_surface->w) / 2,
                                    button.body.y + (button.body.h - text_surface->h) / 2,
                                    f32(text_surface->w),
                                    f32(text_surface->h)};
                SDL_RenderTexture(renderer, text_texture, nullptr, &text_rect);
                SDL_DestroyTexture(text_texture);
            }
            SDL_DestroySurface(text_surface);
        }
    }
}
