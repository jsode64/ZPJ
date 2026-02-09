#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Texture {
private:
    /** @brief A handle to the texture. */
    SDL_Texture* data;

public:
    Texture();

    ~Texture();

    /** @brief Returns a handle to the texture. */
    SDL_Texture* get();

    /** @brief Loads the texture from the given `.bmp` file. */
    void load_bmp(const char* path);

    /** @brief Renders the given text. */
    void load_text(TTF_Font* font, const std::string& text);
};
