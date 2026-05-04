#include "win_menu.h"

#include "assets.h"
#include "game.h"
#include "util.h"
#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>

WinMenu::WinMenu(Game& game) : game{game}, click(KEY_DOWN_MOUSE(SDL_BUTTON_LMASK)) {
    const f32 centerX = f32(gWindow.get_width()) / 2.0f;
    const f32 centerY = f32(gWindow.get_height()) / 2.0f;

    const Button MAIN_MENU_BUTTON{
        {centerX - BUTTON_W / 2.0f, centerY + 50.0f, BUTTON_W, BUTTON_H},
        "MAIN MENU",
        [](Player&, Game& g, Shop&) { g.open_main_menu(); }};

    buttons = {MAIN_MENU_BUTTON};
}

void WinMenu::update(f32 mouseX, f32 mouseY, [[maybe_unused]] bool mouse_clicked) {
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

void WinMenu::draw() const {
    SDL_Renderer* const renderer = gWindow.get_renderer();

    // Draw background overlay (semi-transparent)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_FRect bg{0, 0, f32(gWindow.get_width()), f32(gWindow.get_height())};
    SDL_RenderFillRect(renderer, &bg);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Draw title
    if (gAssets.font) {
        SDL_Surface* title_surface =
            TTF_RenderText_Solid(gAssets.font, "YOU WIN!", 8, SDL_Color{100, 255, 100, 255});
        if (title_surface) {
            SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
            if (title_texture) {
                SDL_FRect title_rect{(f32(gWindow.get_width()) - f32(title_surface->w) * 2.0f) / 2.0f,
                                     f32(gWindow.get_height()) / 2.0f - 100.0f,
                                     f32(title_surface->w) * 2.0f,
                                     f32(title_surface->h) * 2.0f};
                SDL_RenderTexture(renderer, title_texture, nullptr, &title_rect);
                SDL_DestroyTexture(title_texture);
            }
            SDL_DestroySurface(title_surface);
        }
    }

    // Draw buttons
    for (const auto& button : buttons) {
        render_button(button, renderer, gAssets.font);
    }
}
