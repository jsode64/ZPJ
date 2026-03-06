#include "shop.h"

#include "assets.h"
#include "def.h"
#include "game.h"
#include "util.h"
#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

Shop::Shop(Game& game, Player& player) : game{game}, player{player}, click(KEY_DOWN_MOUSE(SDL_BUTTON_LMASK)) {
    // Create buttons.
    const f32 W = (BUTTON_W * 2) + (BUTTON_SPACING * 3);
    const f32 X = (gWindow.get_width() - W) / 2;
    const f32 Y = 150;
    const Button INCREASE_CAPACITY_BUTTON{{X, Y, BUTTON_W, BUTTON_H}, "INCREASE\nCAPACITY", [](Player& p, Game&) {
                                              if (p.take_coins(2)) {
                                                  p.increase_battery_capacity();
                                              }
                                          }};
    const Button EXIT_SHOP_BUTTON{
        {X + BUTTON_W + BUTTON_SPACING, Y, BUTTON_W, BUTTON_H}, "EXIT SHOP", [](Player&, Game& g) { g.start_level(); }};
    const Button INCREASE_JUMP_BUTTON{
        {X, Y + BUTTON_H + BUTTON_SPACING, BUTTON_W, BUTTON_H}, "INCREASE\nJUMP", [](Player& p, Game&) {
            if (p.take_coins(1)) {
                p.increase_jump();
            }
        }};

    // Put in array.
    buttons = {
        INCREASE_CAPACITY_BUTTON,
        EXIT_SHOP_BUTTON,
        INCREASE_JUMP_BUTTON,
    };
}

void Shop::update(f32 mouseX, f32 mouseY, bool mouse_clicked) {
    // See if the user clicked.
    click.update();
    if (!click.was_just_pressed()) {
        return;
    }

    // Find which button was clicked.
    for (const auto& button : buttons) {
        if (is_point_in_rect({f32{mouseX}, f32{mouseY}}, button.body)) {
            button.cb(player, game);
            return;
        }
    }
}

void Shop::draw() const {
    SDL_Renderer* const renderer = gWindow.get_renderer();

    // Draw background.
    SDL_SetRenderDrawColor(renderer, 30, 30, 60, 255);
    SDL_FRect bg{0, 0, static_cast<f32>(gWindow.get_width()), static_cast<f32>(gWindow.get_height())};
    SDL_RenderFillRect(renderer, &bg);

    // Draw title
    if (gAssets.font) {
        SDL_Surface* title_surface = TTF_RenderText_Solid(gAssets.font, "SHOP", 8, SDL_Color{255, 255, 100, 255});
        if (title_surface) {
            SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
            if (title_texture) {
                SDL_FRect title_rect{
                    f32(gWindow.get_width() - title_surface->w) / 2, 20, f32(title_surface->w), f32(title_surface->h)};
                SDL_RenderTexture(renderer, title_texture, nullptr, &title_rect);
                SDL_DestroyTexture(title_texture);
            }
            SDL_DestroySurface(title_surface);
        }
    }

    // Draw player stats.
    const std::string stats =
        "Coins: " + std::to_string(player.get_coins()) + " | Battery: " + std::to_string(player.get_battery_capacity());
    const SDL_FRect dst{
        0.0f,
        100.0f,
        static_cast<f32>(gWindow.get_width()),
        static_cast<f32>(gWindow.get_height()) / 12.0f,
    };
    Texture texture{gWindow.create_text(stats)};
    SDL_RenderTexture(renderer, texture.get(), nullptr, &dst);

    // Draw buttons
    for (const auto& button : buttons) {
        render_button(button, renderer, gAssets.font);
    }
}
