#pragma once

#include <SDL3/SDL.h>

class Key {
private:
  /** The key being tracked. */
  SDL_Scancode key;

  /** Is `true` if the key is down now, `false` if not. */
  bool isDown;

  /** Is `true` if the key was just pressed, `false` if not. */
  bool wasJustPressed;

public:
  constexpr Key(SDL_Scancode key) : key(key), isDown(false), wasJustPressed(false) {}

  /** Returns `true` if the key is down, `false` if not. */
  bool is_down() const;

  /** Returns `true` if the key was just pressed, `false` if not. */
  bool was_just_pressed() const;

  /** Updates the key state. */
  void update();
};
