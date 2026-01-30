#include "game.h"

Game::Game() : player(), world() {

}

Game::~Game() {

}

void Game::start_level() {
    world.init();
    player.init();
}

void Game::update() {
    player.update(world);

    // Restart if player is out of battery.
    if (player.is_out_of_battery()) {
        start_level();
    }
}

void Game::draw(Window& window) const {
    world.draw(window, player);
    player.draw(window);
}
