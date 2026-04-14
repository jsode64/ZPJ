#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <tuple>

class Texture {
  private:
    /** A handle to the texture. */
    SDL_Texture* data;

  public:
    constexpr Texture() : data{nullptr} {}

    constexpr Texture(SDL_Texture* data) : data{data} {}

    Texture(const char* path);

    ~Texture();

    /** Returns a handle to the texture. */
    SDL_Texture* get() const;

    /** Loads the texture from the given `.bmp` file. */
    void load_bmp(const char* path);

    /** Loads the texture from the given `.png` file. */
    void load_png(const char* path);

    /** Renders the given text. */
    void load_text(TTF_Font* font, const std::string& text);
};
