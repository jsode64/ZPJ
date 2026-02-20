#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include "player.h"
#include "shop.h"
#include "texture.h"
#include "window.h"
#include "world.h"

class Game {
private:
  enum class State {
    Level,
    Shop,
  };

  /** The player. */
  Player player;

  /** The world. */
  World world;

  /** The shop menu. */
  Shop shop;

  /** The current game state. */
  State state;

  /** Initializes the level. */
  void init_level();

public:
  Game();

  /** Updates the game. */
  void update();

  /** Draws the game. */
  void draw() const;
};
