#include "pause_menu.h"

#include "assets.h"
#include "game.h"
#include "util.h"
#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>

PauseMenu::PauseMenu(Game& game) : game{game}, click(KEY_DOWN_MOUSE(SDL_BUTTON_LMASK)) {
    // Center the buttons on screen.
    const f32 totalH = (BUTTON_H * 3) + (BUTTON_SPACING * 2);
    const f32 startX = (f32(gWindow.get_width()) - BUTTON_W) / 2.0f;
    const f32 startY = (f32(gWindow.get_height()) - totalH) / 2.0f + 40.0f;

    const Button RESUME_BUTTON{
        {startX, startY, BUTTON_W, BUTTON_H}, "RESUME", [](Player&, Game& g, Shop&) { g.resume(); }};

    const Button SETTINGS_BUTTON{
        {startX, startY + BUTTON_H + BUTTON_SPACING, BUTTON_W, BUTTON_H},
        "SETTINGS",
        [](Player&, Game& g, Shop&) { g.open_settings(); }};

    const Button EXIT_BUTTON{
        {startX, startY + (BUTTON_H + BUTTON_SPACING) * 2, BUTTON_W, BUTTON_H},
        "EXIT",
        [](Player&, Game& g, Shop&) { g.open_main_menu(); }};

    buttons = {RESUME_BUTTON, SETTINGS_BUTTON, EXIT_BUTTON};
}

void PauseMenu::update(f32 mouseX, f32 mouseY, [[maybe_unused]] bool mouse_clicked) {
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

void PauseMenu::draw() const {
    SDL_Renderer* const renderer = gWindow.get_renderer();

    // Draw semi-transparent background overlay.
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_FRect bg{0, 0, f32(gWindow.get_width()), f32(gWindow.get_height())};
    SDL_RenderFillRect(renderer, &bg);

    // Draw title "PAUSED"
    if (gAssets.font) {
        SDL_Surface* title_surface = TTF_RenderText_Solid(gAssets.font, "PAUSED", 6, SDL_Color{255, 255, 100, 255});
        if (title_surface) {
            SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
            if (title_texture) {
                SDL_FRect title_rect{(f32(gWindow.get_width()) - f32(title_surface->w) * 1.5f) / 2.0f,
                                     100.0f,
                                     f32(title_surface->w) * 1.5f,
                                     f32(title_surface->h) * 1.5f};
                SDL_RenderTexture(renderer, title_texture, nullptr, &title_rect);
                SDL_DestroyTexture(title_texture);
            }
            SDL_DestroySurface(title_surface);
        }
    }

    // Draw buttons.
    for (const auto& button : buttons) {
        render_button(button, renderer, gAssets.font);
    }
}
