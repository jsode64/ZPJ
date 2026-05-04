#include "tutorial_menu.h"

#include "assets.h"
#include "game.h"
#include "util.h"
#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>

TutorialMenu::TutorialMenu(Game& game) : game{game}, click(KEY_DOWN_MOUSE(SDL_BUTTON_LMASK)) {
    const f32 centerX = f32(gWindow.get_width()) / 2.0f;
    const f32 centerY = f32(gWindow.get_height()) / 2.0f;

    const Button BACK_BUTTON{
        {centerX - BACK_BUTTON_W / 2.0f, centerY + 130.0f, BACK_BUTTON_W, BACK_BUTTON_H},
        "BACK",
        [](Player&, Game& g, Shop&) { g.close_tutorial(); }};

    buttons = {BACK_BUTTON};
}

void TutorialMenu::update(f32 mouseX, f32 mouseY, [[maybe_unused]] bool mouse_clicked) {
    click.update();

    if (!click.was_just_pressed()) {
        return;
    }

    for (const auto& button : buttons) {
        if (is_point_in_rect({mouseX, mouseY}, button.body)) {
            button.cb(game.get_player(), game, game.get_shop());
            return;
        }
    }
}

void TutorialMenu::draw() const {
    SDL_Renderer* const renderer = gWindow.get_renderer();

    // Draw background.
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_FRect bg{0, 0, f32(gWindow.get_width()), f32(gWindow.get_height())};
    SDL_RenderFillRect(renderer, &bg);

    if (gAssets.font) {
        SDL_Surface* title_surface =
            TTF_RenderText_Solid(gAssets.font, "TUTORIAL", 8, SDL_Color{255, 255, 100, 255});
        if (title_surface) {
            SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
            if (title_texture) {
                SDL_FRect title_rect{(f32(gWindow.get_width()) - f32(title_surface->w) * 2.0f) / 2.0f,
                                     60.0f,
                                     f32(title_surface->w) * 2.0f,
                                     f32(title_surface->h) * 2.0f};
                SDL_RenderTexture(renderer, title_texture, nullptr, &title_rect);
                SDL_DestroyTexture(title_texture);
            }
            SDL_DestroySurface(title_surface);
        }
    }

    if (gAssets.font) {
        const char* lines[] = {
            "The goal of the game is to collect all 5 fruits on the map and reach the exit door.",
            "Navigate around the map while collecting coins in order to purchase upgrades in the shop.",
            "These upgrades will allow you to get to new areas, and give you more time to reach the end."
        };

        const f32 spacing = 15.0f;
        f32 currentY = f32(gWindow.get_height()) / 2.0f - 80.0f;

        for (const char* line : lines) {
            size_t len = 0;
            while (line[len] != '\0') len++; // Calculate string length safely
            
            SDL_Surface* text_surface =
                TTF_RenderText_Solid(gAssets.font, line, len, SDL_Color{255, 255, 255, 255});
            if (text_surface) {
                SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
                if (text_texture) {
                    SDL_FRect text_rect{(f32(gWindow.get_width()) - f32(text_surface->w)) / 2.0f,
                                         currentY,
                                         f32(text_surface->w),
                                         f32(text_surface->h)};
                    SDL_RenderTexture(renderer, text_texture, nullptr, &text_rect);
                    SDL_DestroyTexture(text_texture);
                }
                currentY += f32(text_surface->h) + spacing;
                SDL_DestroySurface(text_surface);
            }
        }
    }

    for (const auto& button : buttons) {
        render_button(button, renderer, gAssets.font);
    }
}
