#include "main_menu.h"

#include "assets.h"
#include "game.h"
#include "util.h"
#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>

MainMenu::MainMenu(Game& game) : game{game}, click(KEY_DOWN_MOUSE(SDL_BUTTON_LMASK)) {
    const f32 totalH = (BUTTON_H * 3) + (BUTTON_SPACING * 2);
    const f32 startX = (f32(gWindow.get_width()) - BUTTON_W) / 2.0f;
    const f32 startY = (f32(gWindow.get_height()) - totalH) / 2.0f + 40.0f;

    const Button START_BUTTON{
        {startX, startY, BUTTON_W, BUTTON_H}, "START", [](Player&, Game& g, Shop&) { g.start_level(); }};

    const Button SETTINGS_BUTTON{
        {startX, startY + BUTTON_H + BUTTON_SPACING, BUTTON_W, BUTTON_H},
        "SETTINGS",
        [](Player&, Game& g, Shop&) { g.open_settings(); }};

    const Button TUTORIAL_BUTTON{
        {startX, startY + (BUTTON_H + BUTTON_SPACING) * 2.0f, BUTTON_W, BUTTON_H},
        "TUTORIAL",
        [](Player&, Game& g, Shop&) { g.open_tutorial(); }};

    buttons = {START_BUTTON, SETTINGS_BUTTON, TUTORIAL_BUTTON};
}

void MainMenu::update(f32 mouseX, f32 mouseY, [[maybe_unused]] bool mouse_clicked) {
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

void MainMenu::draw() const {
    SDL_Renderer* const renderer = gWindow.get_renderer();

    // Draw background.
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_FRect bg{0, 0, f32(gWindow.get_width()), f32(gWindow.get_height())};
    SDL_RenderFillRect(renderer, &bg);

    // Draw title 
    if (gAssets.font) {
        SDL_Surface* title_surface = TTF_RenderText_Solid(gAssets.font, "0% Juice", 8, SDL_Color{255, 255, 100, 255});
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

    // Draw buttons.
    for (const auto& button : buttons) {
        render_button(button, renderer, gAssets.font);
    }
}
