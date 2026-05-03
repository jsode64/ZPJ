#include "assets.h"

#include <exception>
#include <format>
#include "config.hpp.in"

Assets gAssets;

void Assets::load_assets() {
    const auto FONT_PATH = make_path("comic_sans.ttf");
    if (!TTF_Init() || !(font = TTF_OpenFont(FONT_PATH.data(), 16.0f))) {
        const auto e = std::format("Failed to load font from `{}`: \"{}\"", FONT_PATH, SDL_GetError());
        throw std::runtime_error(e);
    }

    jumpSound.load(make_path("jump.mp3").c_str());
    coinCollectSound.load(make_path("coin.wav").c_str());

    menuText.load_text(font, "Press ENTER to continue!");

    player.load_png(make_path("player.png").c_str());
    coin.load_png(make_path("coin.png").c_str());
    doubleJump.load_png(make_path("doublejump.png").c_str());
    dash.load_png(make_path("dash.png").c_str());
    fruit.load_png(make_path("fruit.png").c_str());
}
