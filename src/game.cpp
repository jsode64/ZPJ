#include "game.h"

#include "audio.h"

Game::Game() : menuText{}, player(), world(), state(State::Level) {

}

Game::~Game() {
    SDL_DestroyTexture(menuText);
}

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
        }
    } else if (state == State::Shop) {
        const auto keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_RETURN]) {
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
        const SDL_FRect dst(
            0.0f,
            0.0f,
            f32(gWindow.get_width()),
            50.0f
        );
        SDL_RenderTexture(gWindow.get_renderer(), menuText, nullptr, &dst);
    }
}
