#pragma once

#include <SDL3/SDL.h>
#include <cmath>
#include <functional>

class Tile {
  public:
    using UpdateFn = Tile (*)(const Tile&);

  private:
    /** The tile's body. */
    SDL_FRect body;

    /** The tile's update function. */
    UpdateFn updateFn;

  public:
    constexpr Tile() : body{{}}, updateFn{nullptr} {}

    constexpr Tile(SDL_FRect body, UpdateFn updateFn) : body{body}, updateFn{updateFn} {}

    /** Returns the tile's update function. */
    UpdateFn get_update_fn() const;

    /** Returns the tile's body. */
    SDL_FRect get_body() const;

    /** Updates the tile. */
    void update();
};

#define TILE_CYCLE(begX, begY, endX, endY, t)                                                                          \
    [](const Tile& tile) {                                                                                             \
        const auto midX = (begX + endX) / 2.0f;                                                                        \
        const auto midY = (begY + endY) / 2.0f;                                                                        \
        const auto rangeX = (endX - begX) / 2.0f;                                                                      \
        const auto rangeY = (endY - begY) / 2.0f;                                                                      \
        const auto src = tile.get_body();                                                                              \
        const auto ticks = SDL_GetTicks();                                                                             \
        const f32 m = std::sin(static_cast<f32>(ticks) / (t * 500.0f));                                                \
        const SDL_FRect dst{midX + (rangeX * m), midY + (rangeY * m), src.w, src.h};                                   \
        return Tile(dst, tile.get_update_fn());                                                                        \
    }
