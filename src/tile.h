#pragma once

#include "def.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <functional>

class Tile {
  public:
    using UpdateFn = void (*)(Tile&);

  private:
    /** The tile's body. */
    SDL_FRect body;

    /** The tile's velocity. */
    SDL_FPoint v;

    /** The tile's update function. */
    UpdateFn updateFn;

    /** Whether this tile damages the player on contact. */
    bool damageable;

    /** Cooldown timer in ticks for damage. */
    mutable i32 damagableCooldown;

  public:
    constexpr Tile() : body{{}}, v{{}}, updateFn{nullptr}, damageable{false}, damagableCooldown{0} {}

    constexpr Tile(SDL_FRect body, UpdateFn updateFn, bool damageable) : body{body}, v{{}}, updateFn{updateFn}, damageable{damageable}, damagableCooldown{0} {}

    /** Returns the tile's update function. */
    UpdateFn get_update_fn() const;

    /** Returns the tile's body. */
    SDL_FRect get_body() const;

    /** Returns the tile's velocity. */
    SDL_FPoint get_v() const;

    /** Sets the tile's velocity. */
    void set_v(SDL_FPoint v);

    /** Returns whether this tile damages the player. */
    bool is_damageable() const;

    /** Sets whether this tile damages the player. */
    void set_damageable(bool dmg);

    /** Resets the damage cooldown. */
    void reset_damage_cooldown() const;

    /** Updates the tile. */
    void update();
};

#define TILE_CYCLE(begX, begY, endX, endY, t)                                                                          \
    [](Tile& tile) {                                                                                                   \
        const auto midX = (begX + endX) / 2.0f;                                                                        \
        const auto midY = (begY + endY) / 2.0f;                                                                        \
        const auto rangeX = (endX - begX) / 2.0f;                                                                      \
        const auto rangeY = (endY - begY) / 2.0f;                                                                      \
        const auto src = tile.get_body();                                                                              \
        const auto ticks = SDL_GetTicks();                                                                             \
        const f32 m = std::sin(static_cast<f32>(ticks) / (t * 500.0f));                                                \
        const auto body = tile.get_body();                                                                             \
        const SDL_FPoint v{                                                                                            \
            (midX + (rangeX * m)) - body.x,                                                                            \
            (midY + (rangeY * m)) - body.y,                                                                            \
        };                                                                                                             \
        tile.set_v(v);                                                                                                 \
    }
