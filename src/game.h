#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include "main_menu.h"
#include "pause_menu.h"
#include "player.h"
#include "settings_menu.h"
#include "shop.h"
#include "texture.h"
#include "tutorial_menu.h"
#include "window.h"
#include "win_menu.h"
#include "world.h"

class Game {
  private:
    enum class State {
        MainMenu,
        SettingsMenu,
        TutorialMenu,
        WinMenu,
        Paused,
        Level,
        Shop,
    };

    /** The player. */
    Player player;

    /** The world. */
    World world;

    /** The shop menu. */
    Shop shop;

    /** The main menu. */
    MainMenu main_menu;

    /** The settings menu. */
    SettingsMenu settings_menu;

    /** The tutorial menu. */
    TutorialMenu tutorial_menu;

    /** The win menu. */
    WinMenu win_menu;

    /** The pause menu. */
    PauseMenu pause_menu;

    /** The current game state. */
    State state;

    /** The previous state (used for settings menu return). */
    State previous_state;

    /** Pause key (Escape). */
    Key pause_key;

  public:
    Game();

    /** Sets the game state to level. */
    void start_level();

    /** Opens the settings menu. */
    void open_settings();

    /** Closes the settings menu and returns to previous state. */
    void close_settings();

    /** Opens the tutorial menu. */
    void open_tutorial();

    /** Closes the tutorial menu and returns to previous state. */
    void close_tutorial();

    /** Opens the win menu. */
    void open_win_menu();

    /** Opens the main menu. */
    void open_main_menu();

    /** Resumes the game from pause. */
    void resume();

    /** Toggles the pause state. */
    void toggle_pause();

    /** Returns the player. */
    Player& get_player() { return player; }

    /** Returns the shop. */
    Shop& get_shop() { return shop; }

    /** Updates the game. */
    void update();

    /** Draws the game. */
    void draw() const;
};
