#pragma once

#include <SDL3/SDL.h>
#include <functional>

using KeyDownFn = std::function<bool(void)>;

#define KEY_DOWN_SCANCODE(scancode) []() { return SDL_GetKeyboardState(nullptr)[(scancode)]; }
#define KEY_DOWN_MOUSE(mb) []() { return (SDL_GetGlobalMouseState(nullptr, nullptr) & (mb)) != 0; }

class Key {
  private:
    /** The function to tell if the key is down. */
    KeyDownFn keyDownFn;

    /** Is `true` if the key is down now, `false` if not. */
    bool isDown;

    /** Is `true` if the key was just pressed, `false` if not. */
    bool wasJustPressed;

  public:
    Key(KeyDownFn keyDownFn);

    /** Returns `true` if the key is down, `false` if not. */
    bool is_down() const;

    /** Returns `true` if the key was just pressed, `false` if not. */
    bool was_just_pressed() const;

    /** Updates the key state. */
    void update();
};
