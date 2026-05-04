#pragma once

#include "def.h"
#include "key.h"
#include "menu.h"
#include <SDL3/SDL.h>

class Game;

class SettingsMenu : public Menu {
  private:
    /** Slider track dimensions. */
    static constexpr f32 SLIDER_W = 300.0f;
    static constexpr f32 SLIDER_H = 10.0f;

    /** Slider handle dimensions. */
    static constexpr f32 HANDLE_W = 16.0f;
    static constexpr f32 HANDLE_H = 30.0f;

    /** The width of the back button. */
    static constexpr f32 BACK_BUTTON_W = 200.0f;

    /** The height of the back button. */
    static constexpr f32 BACK_BUTTON_H = 50.0f;

    /** The space between elements. */
    static constexpr f32 SPACING = 30.0f;

    /** The game. */
    Game& game;

    /** Click state. */
    Key click;

    /** Current volume level (0.0 to 1.0). */
    f32 volume;

    /** Is the slider handle currently being dragged? */
    bool dragging;

    enum class RebindMode { None, Left, Right, Jump, Dash };
    RebindMode currentRebind;

    /** Returns the slider track rect (centered on screen). */
    SDL_FRect get_slider_track() const;

    /** Returns the slider handle rect based on current volume. */
    SDL_FRect get_slider_handle() const;

  public:
    SettingsMenu(Game& game);

    /** Update settings state (handle mouse input). */
    void update(f32 mouse_x, f32 mouse_y, bool mouse_clicked) override;

    /** Draw the settings menu. */
    void draw() const override;
};
