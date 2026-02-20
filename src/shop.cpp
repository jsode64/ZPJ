#include "shop.h"
#include "assets.h"
#include "def.h"
#include "window.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

Shop::Shop() : player_coins(0), player_battery(100), last_clicked_button(-1) {

  // Create buttons
  buttons.resize(3);

  // Layout buttons horizontally
  f32 total_width = (BUTTON_WIDTH * 3) + (BUTTON_SPACING * 2);
  f32 start_x = (800 - total_width) / 2; // Center horizontally (assuming 800 width)
  f32 start_y = 150;

  // Restore Battery button
  buttons[0].rect = SDL_FRect{start_x, start_y, f32(BUTTON_WIDTH), f32(BUTTON_HEIGHT)};
  buttons[0].label = "Restore\nBattery";
  buttons[0].id = static_cast<int>(ButtonID::RestoreBattery);
  buttons[0].hovered = false;

  // Exit Shop button
  buttons[1].rect = SDL_FRect{start_x + (BUTTON_WIDTH + BUTTON_SPACING) * 2, start_y,
                              f32(BUTTON_WIDTH), f32(BUTTON_HEIGHT)};
  buttons[1].label = "Exit Shop";
  buttons[1].id = static_cast<int>(ButtonID::Exit);
  buttons[1].hovered = false;
}

void Shop::update(i32 mouse_x, i32 mouse_y, bool mouse_clicked) {
  // Reset hover state
  for (auto& button : buttons) {
    button.hovered = false;
  }

  // Check which button is hovered
  for (auto& button : buttons) {
    if (point_in_rect(f32(mouse_x), f32(mouse_y), button.rect)) {
      button.hovered = true;

      // Handle click
      if (mouse_clicked) {
        last_clicked_button = button.id;

        // Handle button actions
        switch (static_cast<ButtonID>(button.id)) {
        case ButtonID::RestoreBattery:
          if (player_coins >= 50) {
            player_coins -= 50;
            player_battery = 100;
          }
          break;
        case ButtonID::Exit:
          // Exit will be handled by the game
          break;
        }
      }
    }
  }
}

void Shop::draw(SDL_Renderer* renderer) const {
  // Draw background
  SDL_SetRenderDrawColor(renderer, 30, 30, 60, 255);
  SDL_FRect bg{0, 0, static_cast<f32>(gWindow.get_width()), static_cast<f32>(gWindow.get_height())};
  SDL_RenderFillRect(renderer, &bg);

  // Draw title
  if (gAssets.font) {
    SDL_Surface* title_surface =
        TTF_RenderText_Solid(gAssets.font, "SHOP", 8, SDL_Color{255, 255, 100, 255});
    if (title_surface) {
      SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
      if (title_texture) {
        SDL_FRect title_rect{f32(gWindow.get_width() - title_surface->w) / 2, 20,
                             f32(title_surface->w), f32(title_surface->h)};
        SDL_RenderTexture(renderer, title_texture, nullptr, &title_rect);
        SDL_DestroyTexture(title_texture);
      }
      SDL_DestroySurface(title_surface);
    }
  }

  // Draw player stats
  if (gAssets.font) {
    std::string stats =
        "Coins: " + std::to_string(player_coins) + " | Battery: " + std::to_string(player_battery);
    SDL_Surface* stats_surface = TTF_RenderText_Solid(
        gAssets.font, stats.c_str(), (stats.length() + 1), SDL_Color{200, 200, 200, 255});
    if (stats_surface) {
      SDL_Texture* stats_texture = SDL_CreateTextureFromSurface(renderer, stats_surface);
      if (stats_texture) {
        SDL_FRect stats_rect{f32(gWindow.get_width() - stats_surface->w) / 2, 100,
                             f32(stats_surface->w), f32(stats_surface->h)};
        SDL_RenderTexture(renderer, stats_texture, nullptr, &stats_rect);
        SDL_DestroyTexture(stats_texture);
      }
      SDL_DestroySurface(stats_surface);
    }
  }

  // Draw buttons
  for (const auto& button : buttons) {
    render_button(button, renderer, gAssets.font);
  }
}

void Shop::set_player_stats(int coins, int battery) {
  player_coins = coins;
  player_battery = battery;
}

int Shop::get_last_action() {
  int action = last_clicked_button;
  last_clicked_button = -1; // Reset after retrieving
  return action;
}
