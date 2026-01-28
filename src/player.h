#pragma once

#include <SDL3/SDL.h>

#include "def.h"
#include "window.h"

#define X_SPEED 30.0f
#define Y_SPEED 50.0f
#define GRAVITY 5.0f

class Player {
private:
    SDL_FRect body;
    
    SDL_FPoint v;

public:
    /** @brief Player width. */
    static constexpr f32 W = 32.0f;

    /** @brief Player height. */
    static constexpr f32 H = 32.0f;

    Player();

    ~Player();

    /** @brief Returns the player's body. */
    SDL_FRect get_body() const;

    /** @brief Updates the player (handles movement and collision). */
    void update();

    /** @brief Draws the player. */
    void draw(Window& window) const;
};
