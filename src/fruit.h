#pragma once

#include "def.h"

class Fruit {
  private:
    /** The fruit's x-position. */
    f32 x;

    /** The fruit's y-position. */
    f32 y;

    /** Wheter the fruit is active (not collected). */
    bool isActive;

  public:
    /** The width of a fruit. */
    static constexpr f32 W = 32.0f;

    /** The height of a fruit. */
    static constexpr f32 H = 32.0f;

    constexpr Fruit() : x{-10'000'000}, y{-10'000'000}, isActive(true) {}

    Fruit(f32 x, f32 y, bool isActive = true);

    /** Returns the fruit's x-position. */
    f32 get_x() const;

    /** Returns the fruit's y-position. */
    f32 get_y() const;

    /** Returns `true` if the fruit is active (was initialized and hasn'e been
     * collected), `false` if not. */
    bool is_active() const;

    /** Collects the fruit. */
    void collect();
};
