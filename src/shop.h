#pragma once

#include "def.h"
#include "key.h"
#include "menu.h"
#include "player.h"
#include <SDL3/SDL.h>

class Game;

class Shop : public Menu {
  private:
    /** The width of each button. */
    static constexpr f32 BUTTON_W = 150.0f;

    /** The height of each button. */
    static constexpr f32 BUTTON_H = 50.0f;

    /** The space between each button. */
    static constexpr f32 BUTTON_SPACING = 30.0f;

    /** The game. */
    Game& game;

    /** The player. */
    Player& player;

    /** Click. */
    Key click;

    // Cost of upgrading the battery capacity.
    i32 batteryCapacityCost = 1;

    // Cost of upgrading the jump.
    i32 jumpUpgradeCost = 1;

  public:
    Shop(Game& game, Player& player);

    /** Update shop state (handle mouse input). */
    void update(f32 mouse_x, f32 mouse_y, bool mouse_clicked) override;

    /** Draw the shop menu. */
    void draw() const override;
};