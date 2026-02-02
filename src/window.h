#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "def.h"

class Window {
private:
    /** @brief The window's SDL window handle. */
    SDL_Window* window;

    /** @brief The window's SDL renderer handle. */
    SDL_Renderer* renderer;

    /** @brief The window's font. */
    TTF_Font* font;

    /** @brief The window's width in pixels. */
    i32 w;

    /** @brief The window's height in pixels. */
    i32 h;

    /** @brief Is 'true` if the window should close (was closed, hit an error), `false` if not. */
    bool shouldClose;

public:
    Window(const char* title, const char* fontPath, i32 w, i32 h);

    ~Window();

    /** @brief Returns the window's SDL renderer handle. */
    SDL_Renderer* get_renderer();

    /** @brief Returns the window's width. */
    i32 get_width() const;

    /** @brief Returns the window's height. */
    i32 get_height() const;

    /** @brief Creates the given text rendered in a texture. */
    SDL_Texture* create_text(const std::string& text);

    /** @brief Returns a texture made from the given file path. */
    SDL_Texture* create_texture(const char* path);

    /** @brief Returns `true` if the window should close (was closed, hit an error), `false` if not. */
    bool should_close() const;

    /** @brief Updates the window state and presents the latest render. */
    void update();
};
