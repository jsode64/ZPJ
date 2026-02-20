#include "key.h"

bool Key::is_down() const { return isDown; }

bool Key::was_just_pressed() const { return wasJustPressed; }

void Key::update() {
  const bool isDownNow = SDL_GetKeyboardState(nullptr)[key];

  wasJustPressed = isDownNow && !isDown;
  isDown = isDownNow;
}
