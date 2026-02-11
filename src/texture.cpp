#include "texture.h"

#include <exception>
#include <format>

#include "window.h"

Texture::Texture(const char* path) : Texture{} {
    load_bmp(path);
}

Texture::~Texture() {
    SDL_DestroyTexture(data);
}

SDL_Texture* Texture::get() {
    return data;
}

void Texture::load_bmp(const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    data = SDL_CreateTextureFromSurface(gWindow.get_renderer(), surface);
    SDL_DestroySurface(surface);

    if (!data) {
        const auto e = std::format("Failed to load texture from `{}`", path);
        throw std::runtime_error(e);
    }
}

void Texture::load_text(TTF_Font* font, const std::string& text) {
    SDL_Surface* surface = TTF_RenderText_Solid(
        font,
        text.c_str(),
        text.size(),
        { 255, 255, 255, 255 }
    );
    data = SDL_CreateTextureFromSurface(gWindow.get_renderer(), surface);
    SDL_DestroySurface(surface);

    if (!data) {
        const auto e = std::format("Failed to render text \"{}\"", text);
        throw std::runtime_error(e);
    }
}
