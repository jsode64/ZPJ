#pragma once

#include <SDL3/SDL.h>

#include "def.h"
#include "key.h"
#include "window.h"
#include "world.h"

class World;

class Player {
private:
  /** Player width. */
  static constexpr f32 W = 32.0f;

  /** Player height. */
  static constexpr f32 H = 32.0f;

  /** The player's x-speed. */
  static constexpr f32 BASE_X_SPEED = 6.0f;

  /** The player's jump speed. */
  static constexpr f32 BASE_JUMP_SPEED = 15.0f;

  /** The player's dash speed. */
  static constexpr f32 DASH_SPEED = 30.0f;

  /** The base battery cost. */
  static constexpr i32 BASE_BATTERY_COST = 10;

  /** The starting battery capacity before upgrades in ticks. */
  static constexpr i32 STARTING_BATTERY_CAPACITY = BASE_BATTERY_COST * 30;

  /** The left key. */
  static constexpr SDL_Scancode LEFT_KEY = SDL_SCANCODE_A;

  /** The right key. */
  static constexpr SDL_Scancode RIGHT_KEY = SDL_SCANCODE_D;

  /** The jump key. */
  static constexpr SDL_Scancode JUMP_KEY = SDL_SCANCODE_SPACE;

  /** The dash key. */
  static constexpr SDL_Scancode DASH_KEY = SDL_SCANCODE_LSHIFT;

  /** The jump key state. */
  Key jumpKeyState;

  /** The dash key state. */
  Key dashKeyState;

  /** The player's body. */
  SDL_FRect body;

  /** The player's velocity. */
  SDL_FPoint v;

  /** X speed. */
  f32 xSpeed;

  /** Jump speed. */
  f32 jumpSpeed;

  /** The player's battery capacity. */
  i32 batteryCapacity;

  /** The amount of battery lost each frame. */
  i32 batteryCost;

  /** The battery the player has remaining. */
  i32 batteryRemaining;

  /** The ticks left in the dash cooldown. */
  i32 dashCooldown;

  /** The coins the player has to spend. */
  i32 numCoins;

  /** Is the player on the ground? */
  bool onGround;

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

  /** Handles player collecting coins/upgrades. */
  void handle_collecting(World& world);

public:
  Player();

  ~Player();

  /** Returns the player's body. */
  SDL_FRect get_body() const;

  /** Is the player out of battery? */
  bool is_out_of_battery() const;

  /** Returns the number of coins the player has. */
  i32 get_coins() const;

  /** Returns the player's current battery percentage. */
  i32 get_battery() const;

  /** Increases the player's battery capacity. */
  void increase_battery_capacity();

  /** Increases the player's battery efficiency. */
  void increase_battery_efficiency();

  /** Increases the player's speed. */
  void increase_speed();

  /** Increases the player's jump height. */
  void increase_jump();

  /** Removes the given number of coins from the player. */
  void take_coins(i32 cost);

  /** Initializes the player. */
  void init();

  /** Updates the player (handles movement and collision). */
  void update(World& world);

  /** Draws the player. */
  void draw() const;
};
