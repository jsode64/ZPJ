#pragma once

#include "def.h"
#include "player.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <string>
#include <vector>

class Game;

using ButtonCb = std::function<void(Player&, Game&)>;

struct Button {
    /** Button's body. */
    SDL_FRect body;

    /** Text in the button. */
    std::string label;

    /** The on-click callback. */
    ButtonCb cb;

    Button(SDL_FRect body, const char* label, ButtonCb cb) : body{body}, label{label}, cb{cb} {}
};

class Menu {
  protected:
    std::vector<Button> buttons;

    /** @brief Render a single button. */
    void render_button(const Button& button, SDL_Renderer* renderer, TTF_Font* font) const;

  public:
    Menu();

    virtual ~Menu() = default;

    /** Updates the menu state. */
    virtual void update(f32 mouse_x, f32 mouse_y, bool mouse_clicked) = 0;

    /** Draws the menu. */
    virtual void draw() const = 0;
};