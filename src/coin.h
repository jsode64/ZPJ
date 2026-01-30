#pragma once

#include <SDL3/SDL.h>

#include "def.h"

class Coin {
private:
    /** @brief The coin's x-position. */
    f32 x;

    /** @brief The coin's y-position. */
    f32 y;

    /** @brief Is `true` if the coin is active (was initialized and hasn'e been collected), `false` if not. */
    bool isActive;

public:
    /** @brief The width of a coin. */
    static constexpr f32 W = 15.0f;

    /** @brief The height of a coin. */
    static constexpr f32 H = 20.0f;

    constexpr Coin(): x{}, y{}, isActive(false) {

    }

    Coin(f32 x, f32 y);

    /** @brief Returns the coin's x-position. */
    f32 get_x() const;

    /** @brief Returns the coin's y-position. */
    f32 get_y() const;

    /** @brief Returns `true` if the coin is active (was initialized and hasn'e been collected), `false` if not. */
    bool is_active() const;

    /** @brief Collects the coin. */
    void collect();
};
