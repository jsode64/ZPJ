#include "game.h"

#include "assets.h"

Game::Game()
    : player(),
      world(),
      shop{*this, player},
      main_menu{*this},
      settings_menu{*this},
      tutorial_menu{*this},
      win_menu{*this},
      pause_menu{*this},
      state{State::MainMenu},
      previous_state{State::MainMenu},
      pause_key{KEY_DOWN_SCANCODE(SDL_SCANCODE_ESCAPE)} {}

void Game::start_level() {
    state = State::Level;
    player.init();
    world.init(player);
}

void Game::open_settings() {
    previous_state = state;
    state = State::SettingsMenu;
}

void Game::close_settings() { state = previous_state; }

void Game::open_tutorial() {
    previous_state = state;
    state = State::TutorialMenu;
}

void Game::close_tutorial() { state = previous_state; }

void Game::open_win_menu() {
    previous_state = state;
    state = State::WinMenu;
}

void Game::open_main_menu() { state = State::MainMenu; }

void Game::resume() { state = State::Level; }

void Game::toggle_pause() {
    if (state == State::Level) {
        state = State::Paused;
    } else if (state == State::Paused) {
        state = State::Level;
    }
}

void Game::update() {
    pause_key.update();
    if (pause_key.was_just_pressed()) {
        toggle_pause();
    }

    f32 mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (state == State::MainMenu) {
        main_menu.update(mouseX, mouseY, false);
    } else if (state == State::SettingsMenu) {
        settings_menu.update(mouseX, mouseY, false);
    } else if (state == State::TutorialMenu) {
        tutorial_menu.update(mouseX, mouseY, false);
    } else if (state == State::WinMenu) {
        win_menu.update(mouseX, mouseY, false);
    } else if (state == State::Paused) {
        pause_menu.update(mouseX, mouseY, false);
    } else if (state == State::Level) {
        world.update(player);
        player.update(world);

        if (player.has_completed_level(world)) {
            open_win_menu();
        } else if (player.is_out_of_battery()) {
            state = State::Shop;
        }
    } else if (state == State::Shop) {
        // Update shop menu.
        shop.update(mouseX, mouseY, false);
    }
}

void Game::draw() const {
    if (state == State::MainMenu) {
        main_menu.draw();
    } else if (state == State::SettingsMenu) {
        settings_menu.draw();
    } else if (state == State::TutorialMenu) {
        tutorial_menu.draw();
    } else if (state == State::WinMenu) {
        world.draw(player);
        player.draw();
        win_menu.draw();
    } else if (state == State::Paused) {
        world.draw(player);
        player.draw();
        pause_menu.draw();
    } else if (state == State::Level) {
        world.draw(player);
        player.draw();
    } else if (state == State::Shop) {
        shop.draw();
    }
}
