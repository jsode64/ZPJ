#pragma once

#include <SDL3/SDL.h>

class Key {
private:
    /** @brief The key being tracked. */
    SDL_Scancode key;

    /** @brief Is `true` if the key is down now, `false` if not. */
    bool isDown;

    /** @brief Is `true` if the key was just pressed, `false` if not. */
    bool wasJustPressed;

public:
    constexpr Key(SDL_Scancode key) : key(key), isDown(false), wasJustPressed(false) {

    }

    /** @brief Returns `true` if the key is down, `false` if not. */
    bool is_down() const;

    /** @brief Returns `true` if the key was just pressed, `false` if not. */
    bool was_just_pressed() const;

    /** @brief Updates the key state. */
    void update();
};
