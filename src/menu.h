#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

#include "def.h"

struct Button {
  SDL_FRect rect;    // Position and size
  std::string label; // Button text
  bool hovered;      // Is the mouse over this button?
  int id;            // Unique identifier for the button
};

class Menu {
protected:
  std::vector<Button> buttons;

  /** Check if a point is inside a rectangle. */
  bool point_in_rect(f32 x, f32 y, const SDL_FRect& rect) const;

  /** Render a single button. */
  void render_button(const Button& button, SDL_Renderer* renderer, TTF_Font* font) const;

public:
  Menu();
  virtual ~Menu() = default;

  /** Update the menu state (handle input, etc). */
  virtual void update(i32 mouse_x, i32 mouse_y, bool mouse_clicked) = 0;

  /** Draw the menu. */
  virtual void draw(SDL_Renderer* renderer) const = 0;

  /** Get the ID of the button that was clicked, or -1 if none. */
  int get_clicked_button() const;
};