#include "game.h"


Game::Game(Window& window) : menuText{}, player(), world(), state(State::Level) {
    menuText = window.create_text("PRESS ENTER TO CONTINUE");
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

void Game::draw(Window& window) const {
    if (state == State::Level) {
        world.draw(window, player);
        player.draw(window);
    } else if (state == State::Shop) {
        const SDL_FRect dst(
            0.0f,
            0.0f,
            f32(window.get_width()),
            50.0f
        );
        SDL_RenderTexture(window.get_renderer(), menuText, nullptr, &dst);
    }
}
