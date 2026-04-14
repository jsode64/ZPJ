#pragma once

#include "texture.h"
#include <SDL3/SDL.h>
#include <array>

class FruitPanel {
  private:
    /** The panel background texture. */
    Texture panelTexture;

    /** One icon texture per fruit slot (the first frame of each fruit). */
    Texture iconTextures[5];

    /** Which slots have been collected. */
    bool collected[5];

    /** Panel position on screen. */
    static constexpr float PANEL_X = 16.0f;
    static constexpr float PANEL_Y = 0.0f;

    /** Panel display size. */
    static constexpr float PANEL_W = 200.0f;
    static constexpr float PANEL_H = 100.0f;

    /** Slot centers (x) within the panel, evenly spaced. */
    static constexpr float SLOT_CENTERS[5] = {40.0f, 120.0f, 200.0f, 280.0f, 360.0f};

    /** Slot vertical center within the panel. */
    static constexpr float SLOT_CENTER_Y = 100.0f;

    /** Size of each icon drawn in the slot. */
    static constexpr float ICON_SIZE = 60.0f;

  public:
    constexpr FruitPanel() : panelTexture{}, iconTextures{}, collected{false, false, false, false, false} {}
    ~FruitPanel() = default;

    /** Loads all textures. Call after renderer is ready. */
    void init();

    /** Marks the given slot (0-4) as collected. */
    void collect(int slot);

    /** Draws the panel and any collected fruit icons. */
    void draw() const;
};