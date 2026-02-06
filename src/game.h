#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include "player.h"
#include "window.h"
#include "world.h"

class Game {
private:
    enum class State {
        Level,
        Shop,
    };

    /** @brief The menu text. */
    SDL_Texture* menuText;

    /** @brief The player. */
    Player player;

    /** @brief The world. */
    World world;

    /** @brief The current game state. */
    State state;

    /** @brief Initializes the level. */
    void init_level();

public:
    Game();

    ~Game();

    /** @brief Updates the game. */
    void update();

    /** @brief Draws the game. */
    void draw() const;
};
