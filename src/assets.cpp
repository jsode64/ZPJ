#include "assets.h"

#include <exception>
#include <format>

Assets gAssets;


void Assets::load_assets() {
    if (!TTF_Init() || !(font = TTF_OpenFont(FONT_PATH, 16.0f))) {
        const auto e = std::format("Failed to load font from `{}`: \"{}\"", FONT_PATH, SDL_GetError());
        throw std::runtime_error(e);
    }

    jumpSound.load("assets/jump.mp3");
    coinCollectSound.load("assets/coin.wav");

    menuText.load_text(font, "Press ENTER to continue!");
}
