#include "settings_menu.h"

#include "assets.h"
#include "game.h"
#include "mixer.h"
#include "util.h"
#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <string>

SDL_FRect SettingsMenu::get_slider_track() const {
    const f32 centerX = f32(gWindow.get_width()) / 2.0f;
    const f32 centerY = f32(gWindow.get_height()) / 2.0f - 60.0f;
    return {centerX - SLIDER_W / 2.0f, centerY - SLIDER_H / 2.0f, SLIDER_W, SLIDER_H};
}

SDL_FRect SettingsMenu::get_slider_handle() const {
    const SDL_FRect track = get_slider_track();
    const f32 handleX = track.x + (volume * (track.w - HANDLE_W));
    const f32 handleY = track.y + (track.h / 2.0f) - (HANDLE_H / 2.0f);
    return {handleX, handleY, HANDLE_W, HANDLE_H};
}

SettingsMenu::SettingsMenu(Game& game)
    : game{game}, click(KEY_DOWN_MOUSE(SDL_BUTTON_LMASK)), volume{1.0f}, dragging{false}, currentRebind{RebindMode::None} {
    const f32 centerX = f32(gWindow.get_width()) / 2.0f;
    const f32 centerY = f32(gWindow.get_height()) / 2.0f;

    const f32 startX = centerX - BACK_BUTTON_W - 5.0f;
    const f32 startY = centerY - 20.0f;

    const Button REBIND_LEFT{{startX, startY, BACK_BUTTON_W, BACK_BUTTON_H}, "LEFT: ", [this](Player&, Game&, Shop&) {
                                 currentRebind = RebindMode::Left;
                             }};

    const Button REBIND_RIGHT{
        {startX, startY + BACK_BUTTON_H + 10.0f, BACK_BUTTON_W, BACK_BUTTON_H}, "RIGHT: ", [this](Player&, Game&, Shop&) {
            currentRebind = RebindMode::Right;
        }};

    const Button REBIND_JUMP{
        {startX + BACK_BUTTON_W + 10.0f, startY, BACK_BUTTON_W, BACK_BUTTON_H}, "JUMP: ", [this](Player&, Game&, Shop&) {
            currentRebind = RebindMode::Jump;
        }};

    const Button REBIND_DASH{{startX + BACK_BUTTON_W + 10.0f, startY + BACK_BUTTON_H + 10.0f, BACK_BUTTON_W, BACK_BUTTON_H},
                             "DASH: ",
                             [this](Player&, Game&, Shop&) { currentRebind = RebindMode::Dash; }};

    const Button BACK_BUTTON{
        {centerX - BACK_BUTTON_W / 2.0f, centerY + 130.0f, BACK_BUTTON_W, BACK_BUTTON_H},
        "BACK",
        [](Player&, Game& g, Shop&) { g.close_settings(); }};

    buttons = {REBIND_LEFT, REBIND_RIGHT, REBIND_JUMP, REBIND_DASH, BACK_BUTTON};
}

void SettingsMenu::update(f32 mouseX, f32 mouseY, [[maybe_unused]] bool mouse_clicked) {
    // Update button labels.
    Player& player = game.get_player();
    buttons[0].label = "LEFT: " + std::string(SDL_GetScancodeName(player.get_left_key()));
    buttons[1].label = "RIGHT: " + std::string(SDL_GetScancodeName(player.get_right_key()));
    buttons[2].label = "JUMP: " + std::string(SDL_GetScancodeName(player.get_jump_key()));
    buttons[3].label = "DASH: " + std::string(SDL_GetScancodeName(player.get_dash_key()));

    if (currentRebind != RebindMode::None) {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        for (int i = 0; i < SDL_SCANCODE_COUNT; ++i) {
            if (keys[i]) {
                SDL_Scancode sc = static_cast<SDL_Scancode>(i);
                if (currentRebind == RebindMode::Left) player.set_left_key(sc);
                else if (currentRebind == RebindMode::Right) player.set_right_key(sc);
                else if (currentRebind == RebindMode::Jump) player.set_jump_key(sc);
                else if (currentRebind == RebindMode::Dash) player.set_dash_key(sc);
                currentRebind = RebindMode::None;
                break;
            }
        }
        return;
    }

    click.update();

    const bool mouseDown = click.is_down();
    const bool mouseJustPressed = click.was_just_pressed();

    // Handle slider dragging.
    if (mouseJustPressed) {
        const SDL_FRect handle = get_slider_handle();
        const SDL_FRect track = get_slider_track();
        const SDL_FRect trackHit{track.x, handle.y, track.w, HANDLE_H};

        if (is_point_in_rect({mouseX, mouseY}, handle) || is_point_in_rect({mouseX, mouseY}, trackHit)) {
            dragging = true;
        }
    }

    if (!mouseDown) {
        dragging = false;
    }

    if (dragging) {
        const SDL_FRect track = get_slider_track();
        const f32 relX = mouseX - track.x - (HANDLE_W / 2.0f);
        volume = std::clamp(relX / (track.w - HANDLE_W), 0.0f, 1.0f);
        gMixer.set_volume(volume);
    }

    // Handle button clicks (Back).
    if (mouseJustPressed && !dragging) {
        for (const auto& button : buttons) {
            if (is_point_in_rect({mouseX, mouseY}, button.body)) {
                button.cb(game.get_player(), game, game.get_shop());
                return;
            }
        }
    }
}

void SettingsMenu::draw() const {
    SDL_Renderer* const renderer = gWindow.get_renderer();

    // Draw background.
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_FRect bg{0, 0, f32(gWindow.get_width()), f32(gWindow.get_height())};
    SDL_RenderFillRect(renderer, &bg);


    if (gAssets.font) {
        SDL_Surface* title_surface =
            TTF_RenderText_Solid(gAssets.font, "SETTINGS", 8, SDL_Color{255, 255, 100, 255});
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
        const i32 volumePercent = i32(volume * 100.0f + 0.5f);
        const std::string volText = "Volume: " + std::to_string(volumePercent) + "%";
        SDL_Surface* vol_surface =
            TTF_RenderText_Solid(gAssets.font, volText.c_str(), volText.length(), SDL_Color{255, 255, 255, 255});
        if (vol_surface) {
            SDL_Texture* vol_texture = SDL_CreateTextureFromSurface(renderer, vol_surface);
            if (vol_texture) {
                const SDL_FRect track = get_slider_track();
                SDL_FRect vol_rect{(f32(gWindow.get_width()) - f32(vol_surface->w)) / 2.0f,
                                   track.y - f32(vol_surface->h) - 10.0f,
                                   f32(vol_surface->w),
                                   f32(vol_surface->h)};
                SDL_RenderTexture(renderer, vol_texture, nullptr, &vol_rect);
                SDL_DestroyTexture(vol_texture);
            }
            SDL_DestroySurface(vol_surface);
        }
    }

    // Draw slider track (dark background bar).
    const SDL_FRect track = get_slider_track();
    SDL_SetRenderDrawColor(renderer, 80, 80, 100, 255);
    SDL_RenderFillRect(renderer, &track);

    // Draw filled portion of track.
    const SDL_FRect filled{track.x, track.y, track.w * volume, track.h};
    SDL_SetRenderDrawColor(renderer, 50, 100, 200, 255);
    SDL_RenderFillRect(renderer, &filled);

    // Draw slider handle.
    const SDL_FRect handle = get_slider_handle();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &handle);

    // Draw buttons (Back).
    for (const auto& button : buttons) {
        render_button(button, renderer, gAssets.font);
    }

    // Draw rebinding overlay message.
    if (currentRebind != RebindMode::None && gAssets.font) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_FRect bg{0, 0, f32(gWindow.get_width()), f32(gWindow.get_height())};
        SDL_RenderFillRect(renderer, &bg);

        SDL_Surface* msg_surface =
            TTF_RenderText_Solid(gAssets.font, "PRESS ANY KEY...", 16, SDL_Color{255, 255, 255, 255});
        if (msg_surface) {
            SDL_Texture* msg_texture = SDL_CreateTextureFromSurface(renderer, msg_surface);
            if (msg_texture) {
                SDL_FRect msg_rect{(f32(gWindow.get_width()) - f32(msg_surface->w)) / 2.0f,
                                   (f32(gWindow.get_height()) - f32(msg_surface->h)) / 2.0f,
                                   f32(msg_surface->w),
                                   f32(msg_surface->h)};
                SDL_RenderTexture(renderer, msg_texture, nullptr, &msg_rect);
                SDL_DestroyTexture(msg_texture);
            }
            SDL_DestroySurface(msg_surface);
        }
    }
}
