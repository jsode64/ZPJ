#pragma once

#include "def.h"
#include "key.h"
#include "menu.h"
#include <SDL3/SDL.h>

class Game;

class TutorialMenu : public Menu {
  private:
    /** The width of the back button. */
    static constexpr f32 BACK_BUTTON_W = 200.0f;

    /** The height of the back button. */
    static constexpr f32 BACK_BUTTON_H = 50.0f;

    /** The game. */
    Game& game;

    /** Click state. */
    Key click;

  public:
    TutorialMenu(Game& game);

    /** Update menu state (handle mouse input). */
    void update(f32 mouse_x, f32 mouse_y, bool mouse_clicked) override;

    /** Draw the tutorial menu. */
    void draw() const override;
};
