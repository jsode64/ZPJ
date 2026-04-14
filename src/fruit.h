#pragma once

#include "def.h"
#include <SDL3/SDL.h>

enum class FruitType {
    Orange = 0,
    Pineapple = 1,
    Apple = 2,
    Blueberry = 3,
    Strawberry = 4,
};

class Fruit {
  private:
    /** The fruit's x-position. */
    f32 x;

    /** The fruit's y-position. */
    f32 y;

    /** The type of fruit (determines panel slot and texture). */
    FruitType type;

    /** Is true if the fruit is active (not yet collected). */
    bool isActive;

    /** Current animation frame (0-3). */
    mutable int frame;

    /** Tick counter for animation timing. */
    mutable int animTick;

    /** Tick counter for floating animation. */
    mutable float floatTick;

    /** How many ticks per frame (30 ticks/sec / 2 FPS = 30). */
    static constexpr int TICKS_PER_FRAME = 30;

  public:
    /** The width of a fruit. */
    static constexpr f32 W = 64.0f;

    /** The height of a fruit. */
    static constexpr f32 H = 64.0f;

    /** The amplitude of the floating animation in pixels. */
    static constexpr f32 FLOAT_AMPLITUDE = 5.0f;

    constexpr Fruit() : x{}, y{}, type{FruitType::Orange}, isActive(false), frame(0), animTick(0), floatTick(0.0f) {}

    Fruit(f32 x, f32 y, FruitType type);

    /** Returns the fruit's x-position. */
    f32 get_x() const;

    /** Returns the fruit's y-position. */
    f32 get_y() const;

    /** Returns the fruit's body for collision. */
    SDL_FRect get_body() const;

    /** Returns the fruit type. */
    FruitType get_type() const;

    /** Returns the panel slot index for this fruit. */
    int get_slot() const;

    /** Returns true if the fruit is active. */
    bool is_active() const;

    /** Collects the fruit. */
    void collect();

    /** Updates the animation and float. */
    void update() const;

    /** Returns the current animation frame index (0-3). */
    int get_frame() const;

    /** Returns the current floating Y offset. */
    f32 get_float_offset() const;
};