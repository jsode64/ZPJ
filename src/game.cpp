#include "game.h"

#include "assets.h"

Game::Game() : player(), world(), state(State::Level) {}

void Game::init_level() {
  world.init();
  player.init();
}

void Game::update() {
  if (state == State::Level) {
    player.update(world);

    // Go to shop if the player is out of battery.
    if (player.is_out_of_battery()) {
      state = State::Shop;
      // Update shop with current player stats
      shop.set_player_stats(player.get_coins(), player.get_battery());
    }
  } else if (state == State::Shop) {
    // Get mouse state
    f32 mouse_x_f, mouse_y_f;
    SDL_GetMouseState(&mouse_x_f, &mouse_y_f);
    i32 mouse_x = static_cast<i32>(mouse_x_f);
    i32 mouse_y = static_cast<i32>(mouse_y_f);
    const auto keys = SDL_GetKeyboardState(nullptr);
    bool mouse_clicked = keys[SDL_SCANCODE_RETURN]; // For now, use RETURN to simulate click

    // Update shop menu
    shop.update(mouse_x, mouse_y, mouse_clicked);

    // Check what button was clicked
    int action = shop.get_last_action();
    if (action == 1) { // Exit Shop button ID
      state = State::Level;
      init_level();
    }
  }
}

void Game::draw() const {
  if (state == State::Level) {
    world.draw(player);
    player.draw();
  } else if (state == State::Shop) {
    shop.draw(gWindow.get_renderer());
  }
}
