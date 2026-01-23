#pragma once

#include <SDL3/SDL.h>

#include "def.h"

class Window {
private:
    SDL_Window* window;

    SDL_Renderer* renderer;

    bool running;

public:
    Window(const char* title, i32 w, i32 h);

    ~Window();

    /** @brief Returns `true` if the window hasn't been closed yet, `false` if not. */
    bool is_open() const;

    /** @brief Updates the window state and presents the latest render. */
    void update();
};
