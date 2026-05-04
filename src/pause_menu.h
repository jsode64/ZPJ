#pragma once

#include "def.h"
#include "key.h"
#include "menu.h"
#include <SDL3/SDL.h>

class Game;

class PauseMenu : public Menu {
  private:
    /** The width of each button. */
    static constexpr f32 BUTTON_W = 200.0f;

    /** The height of each button. */
    static constexpr f32 BUTTON_H = 60.0f;

    /** The space between each button. */
    static constexpr f32 BUTTON_SPACING = 20.0f;

    /** The game. */
    Game& game;

    /** Click. */
    Key click;

  public:
    PauseMenu(Game& game);

    /** Update pause menu state (handle mouse input). */
    void update(f32 mouse_x, f32 mouse_y, bool mouse_clicked) override;

    /** Draw the pause menu. */
    void draw() const override;
};
