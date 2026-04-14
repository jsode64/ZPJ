#pragma once

#include "texture.h"
#include <SDL3/SDL.h>
#include <array>

class PowerBar {
  private:
    Texture barTexture;
    Texture juiceTexture;

    /** Width of the bar on screen. */
    static constexpr float BAR_W = 128.0f;

    /** Height of the bar on screen. */
    static constexpr float BAR_H = 500.0f;

    /** Margin from the right edge. */
    static constexpr float MARGIN_RIGHT = 16.0f;

    /** Margin from the bottom edge. */
    static constexpr float MARGIN_BOTTOM = 200.0f;

    /** Juice width inside the bar. */
    static constexpr float JUICE_W = 30.0f;

    /** Number of bubble particles. */
    static constexpr int NUM_PARTICLES = 15;

    /** A single bubble particle. */
    struct Particle {
        float x;
        float y;
        float speedY; // upward speed
        float speedX; // horizontal drift
        float size;
    };

    /** The bubble particles. */
    mutable std::array<Particle, NUM_PARTICLES> particles;

    /** Whether particles have been initialized. */
    mutable bool particlesInit = false;

  public:
    constexpr PowerBar() : barTexture{}, juiceTexture{}, particles{}, particlesInit{false} {}
    ~PowerBar() = default;

    /** Loads textures - call after renderer is ready. */
    void init();

    /** Draws the power bar. */
    void draw(int batteryRemaining, int batteryCapacity) const;
};