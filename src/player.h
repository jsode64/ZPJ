#pragma once

#include "def.h"
#include "key.h"
#include "tile.h"
#include "window.h"
#include <SDL3/SDL.h>
#include <optional>

class World;

class Player {
  private:
    /** Player width. */
    static constexpr f32 W = 32.0f;

    /** Player height. */
    static constexpr f32 H = 32.0f;

    /** The player's x-speed. */
    static constexpr f32 BASE_X_SPEED = 5.0f;

    /** The player's jump speed. */
    static constexpr f32 BASE_JUMP_SPEED = 5.0f;

    /** The base battery cost. */
    static constexpr i32 BASE_BATTERY_COST = 10;

    /** The starting battery capacity before upgrades in ticks. */
    static constexpr i32 STARTING_BATTERY_CAPACITY = BASE_BATTERY_COST * 30;

    /** The left key. */
    static constexpr SDL_Scancode LEFT_KEY = SDL_SCANCODE_LEFT;

    /** The right key. */
    static constexpr SDL_Scancode RIGHT_KEY = SDL_SCANCODE_RIGHT;

    /** The jump key. */
    static constexpr SDL_Scancode JUMP_KEY = SDL_SCANCODE_UP;

    /** The dash key. */
    static constexpr SDL_Scancode DASH_KEY = SDL_SCANCODE_LSHIFT;

    /** The maximum coyote time in ticks. */
    static constexpr i32 MAX_COYOTE_TIME = 8;

    /** The jump key state. */
    Key jumpKeyState;

    /** The dash key state. */
    Key dashKeyState;

    /** The player's body. */
    SDL_FRect body;

    /** The player's velocity. */
    SDL_FPoint v;

    /** The tile the player is standing on (or none). */
    std::optional<const Tile*> ground;

    /** X speed multiplier. **/
    f32 xSpeedMulti;

    /** X speed. */
    f32 xSpeed;

    /** Jump speed multiplier. **/
    f32 jumpSpeedMulti;

    /** Jump speed. */
    f32 jumpSpeed;

    /** Dash speed multiplier. */
    f32 dashSpeedMulti;

    /** Dash speed. */
    f32 dashSpeed;

    /** The player's battery capacity. */
    i32 batteryCapacity;

    /** The amount the battery capacity increases with each upgrade. */
    i32 batteryCapacityIncrease;

    /** The amount of battery lost each frame. */
    i32 batteryCost;

    /** The battery the player has remaining. */
    i32 batteryRemaining;

    /** The ticks left in the dash cooldown. */
    i32 dashCooldown;

    /** The coins the player has to spend. */
    i32 numCoins;

    /** The coyote timing remaining. */
    i32 coyoteTime;

    /** Has the player double jumped since last on the ground? */
    bool hasDoubleJump;

    /** Does the player have the double jump unlocked? */
    bool isDoubleJumpUnlocked;

    /** Does the player have the dash unlocked? */
    bool isDashUnlocked;

    /** Handles user input for movement. */
    void handle_input();

    /** Handles movement and collision. */
    void handle_movement(const World& world);

  public:
    Player();

    ~Player();

    /** Returns the player's body. */
    SDL_FRect get_body() const;

    /** Returns the number of coins the player has. */
    i32 get_coins() const;

    /** Returns the player's battery capacity. */
    i32 get_battery_capacity() const;

    /** Does the player have the dash upgrade? */
    bool has_dash_unlocked() const;

    /** Does the player have the double jump upgrade? */
    bool has_double_jump_unlocked() const;

    /** Is the player on the ground? */
    bool is_on_ground() const;

    /** Is the player alive (not crushed)? */
    bool is_alive() const;

    /** Kills the player by draining battery. */
    void kill();

    /** Is the player out of battery? */
    bool is_out_of_battery() const;

    /** Removes the given number of coins from the player, returning `true` if they had that many
     * coins, `false` if not. */
    bool take_coins(i32 cost);

    /** Gives the given number of coins to the player. */
    void give_coins(i32 _numCoins);

    /** Increases the player's battery capacity. */
    void increase_battery_capacity();

    /** Increases the player's battery capacity upgrade (doubles the increase). */
    void increase_battery_capacity_upgrade();

    /** Increases the player's battery efficiency. */
    void increase_battery_efficiency();

    /** Increases the player's speed. */
    void increase_speed();

    /** Increases the player's jump height. */
    void increase_jump();

    /** Increases the player's dash speed. */
    void increase_dash_speed();

    /** Unlocks the dash. */
    void unlock_dash();

    /** Unlocks the double jump. */
    void unlock_double_jump();

    /** Initializes the player. */
    void init();

    /** Updates the player (handles movement and collision). */
    void update(World& world);

    /** Draws the player. */
    void draw() const;
};
