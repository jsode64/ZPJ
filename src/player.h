#pragma once

#include <SDL3/SDL.h>

#include "def.h"
#include "key.h"
#include "window.h"
#include "world.h"

class World;

class Player {
private:
    /** @brief Player width. */
    static constexpr f32 W = 32.0f;

    /** @brief Player height. */
    static constexpr f32 H = 32.0f;

    /** @brief The player's x-speed. */
    static constexpr f32 X_SPEED = 6.0f;

    /** @brief The player's jump speed. */
    static constexpr f32 JUMP_SPEED = 15.0f;

    /** @brief The player's dash speed. */
    static constexpr f32 DASH_SPEED = 30.0f;

    /** @brief The starting battery capacity before upgrades in ticks. */
    static constexpr i32 STARTING_BATTERY_CAPACITY = 150;

    /** @brief The left key. */
    static constexpr SDL_Scancode LEFT_KEY = SDL_SCANCODE_A;

    /** @brief The right key. */
    static constexpr SDL_Scancode RIGHT_KEY = SDL_SCANCODE_D;

    /** @brief The jump key. */
    static constexpr SDL_Scancode JUMP_KEY = SDL_SCANCODE_SPACE;

    /** @brief The dash key. */
    static constexpr SDL_Scancode DASH_KEY = SDL_SCANCODE_LSHIFT;

    /** @brief The jump key state. */
    Key jumpKeyState;

    /** @brief The dash key state. */
    Key dashKeyState;

    /** @brief The player's body. */
    SDL_FRect body;
    
    /** @brief The player's velocity. */
    SDL_FPoint v;

    /** @brief The player's battery capacity. */
    i32 batteryCapacity;

    /** @brief The battery the player has remaining. */
    i32 batteryRemaining;

    /** @brief The ticks left in the dash cooldown. */
    i32 dashCooldown;

    /** @brief The coins the player has to spend. */
    u32 numCoins;

    /** @brief Is `true` if the player is on the ground, `false` if not. */
    bool onGround;

    /** @brief Is `true` if the player has their double jump, `false` if not. */
    bool hasDoubleJump;

    /** @brief Is `true` if the player has the double jump upgrade, `false` if not. */
    bool isDoubleJumpUnlocked;

    /** @brief Is `true` if the player has the dash unlocked, `false` if not. */
    bool isDashUnlocked;

    /** @brief Handles user input for movement. */
    void handle_input();

    /** @brief Handles movement and collision. */
    void handle_movement(const World& world);

    /** @brief Handles player collecting coins/upgrades. */
    void handle_collecting(World& world);

public:
    Player();

    ~Player();

    /** @brief Returns the player's body. */
    SDL_FRect get_body() const;

    /** @brief Returns `true` if the player is out of battery, `false` if not. */
    bool is_out_of_battery() const;

    /** @brief Initializes the player. */
    void init();

    /** @brief Updates the player (handles movement and collision). */
    void update(World& world);

    /** @brief Draws the player. */
    void draw(Window& window) const;
};
