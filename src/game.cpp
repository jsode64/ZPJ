#include "game.h"

Game::Game() : player(), world(), shop{*this, player}, state{State::Level} { start_level(); }

void Game::start_level() {
    state = State::Level;
    player.init();
    world.init(player);
}

void Game::update() {
    if (state == State::Level) {
        world.update(player);
        player.update(world);

        // Go to shop if the player is out of battery.
        if (player.is_out_of_battery()) {
            state = State::Shop;
        }
    } else if (state == State::Shop) {
        // Get mouse position.
        f32 mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Update shop menu.
        shop.update(mouseX, mouseY, false);
    }
}

void Game::draw() const {
    if (state == State::Level) {
        world.draw(player);
        player.draw();
    } else if (state == State::Shop) {
        shop.draw();
    }
}
