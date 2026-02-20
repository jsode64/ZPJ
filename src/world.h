#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <cassert>
#include <span>

#include "coin.h"
#include "def.h"
#include "player.h"
#include "window.h"

class Player;

class World {
private:
  /** The maximum number of objects that can be in a world. */
  static constexpr usize MAX_NUM_OBJS = 1024;

  /** The world's tiles. */
  std::array<SDL_FRect, MAX_NUM_OBJS> tiles;

  /** The number of tiles in the world. */
  usize numTiles;

  /** The world's coins. */
  std::array<Coin, MAX_NUM_OBJS> coins;

  /** The number of coins in the world. */
  usize numCoins;

  /** Asserts that there's room for the tile and pushes it to the world. */
  constexpr void push_tile(const SDL_FRect tile) {
    assert(numTiles < MAX_NUM_OBJS);

    tiles[numTiles] = tile;
    numTiles++;
  }

  /** Asserts that there's room for the coin and pushes it to the world. */
  constexpr void push_coin(const Coin coin) {
    assert(numTiles < MAX_NUM_OBJS);

    coins[numCoins] = coin;
    numCoins++;
  }

public:
  World();

  ~World();

  /** Returns a span of the world's tiles. */
  std::span<const SDL_FRect> get_tiles() const;

  /** Returns a span of the world's coins. */
  std::span<const Coin> get_coins() const;

  /** Returns a span of references to the world's coins. */
  std::span<Coin> get_coins_mut();

  /** Initializes the world. */
  void init();

  /** Draws the stage from the given player's PoV. */
  void draw(const Player& player) const;
};
