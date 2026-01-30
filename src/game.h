#pragma once

#include "player.h"
#include "window.h"
#include "world.h"

class Game {
private:
    /** @brief The player. */
    Player player;

    /** @brief The world. */
    World world;

    /** @brief Starts the level. */
    void start_level();

public:
    Game();

    ~Game();

    /** @brief Updates the game. */
    void update();

    /** @brief Draws the game. */
    void draw(Window& window) const;
};
