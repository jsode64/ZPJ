#pragma once

#include <SDL3/SDL.h>

#include "def.h"
#include "window.h"
#include "world.h"

class World;

class Player {
private:
    /** @brief The player's body. */
    SDL_FRect body;
    
    /** @brief The player's velocity. */
    SDL_FPoint v;

    /** @brief Is `true` if the player is on the ground, `false` if not. */
    bool onGround;

    /** @brief Handles user input for movement. */
    void handle_input();

    /** @brief Handles movement and collision. */
    void handle_movement(const World& world);

public:
    /** @brief Player width. */
    static constexpr f32 W = 32.0f;

    /** @brief Player height. */
    static constexpr f32 H = 32.0f;

    /** @brief The player's x-speed. */
    static constexpr f32 X_SPEED = 6.0f;

    /** @brief The player's jump-speed. */
    static constexpr f32 JUMP_SPEED = 15.0f;

    Player();

    ~Player();

    /** @brief Returns the player's body. */
    SDL_FRect get_body() const;

    /** @brief Updates the player (handles movement and collision). */
    void update(const World& world);

    /** @brief Draws the player. */
    void draw(Window& window) const;
};
