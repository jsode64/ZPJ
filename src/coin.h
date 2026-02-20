#pragma once

#include <SDL3/SDL.h>

#include "def.h"

class Coin {
private:
  /** The coin's x-position. */
  f32 x;

  /** The coin's y-position. */
  f32 y;

  /** Is `true` if the coin is active (was initialized and hasn'e been
   * collected), `false` if not. */
  bool isActive;

public:
  /** The width of a coin. */
  static constexpr f32 W = 15.0f;

  /** The height of a coin. */
  static constexpr f32 H = 20.0f;

  constexpr Coin() : x{}, y{}, isActive(false) {}

  Coin(f32 x, f32 y);

  /** Returns the coin's x-position. */
  f32 get_x() const;

  /** Returns the coin's y-position. */
  f32 get_y() const;

  /** Returns `true` if the coin is active (was initialized and hasn'e been
   * collected), `false` if not. */
  bool is_active() const;

  /** Collects the coin. */
  void collect();
};
