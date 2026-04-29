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
    
    player.load_png("assets/player.png");
    coin.load_png("assets/coin.png");
    rock.load_png("assets/rock.png");
    grass.load_png("assets/grass_tile.png");
    doubleJump.load_png("assets/doublejump.png");
    dash.load_png("assets/dash.png");
    fruit.load_png("assets/fruit.png");
}
