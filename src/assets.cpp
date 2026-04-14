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
    powerBar.init();
    fruitPanel.init();

    SDL_Log("Loading orange frames...");
    orangeFrames[0].load_bmp("assets/orangeOne.bmp");
    orangeFrames[1].load_bmp("assets/orangeTwo.bmp");
    orangeFrames[2].load_bmp("assets/orangeThree.bmp");
    orangeFrames[3].load_bmp("assets/orangeFour.bmp");

    SDL_Log("Loading pineapple frames...");
    pineappleFrames[0].load_bmp("assets/pineappleOne.bmp");
    pineappleFrames[1].load_bmp("assets/pineappleTwo.bmp");
    pineappleFrames[2].load_bmp("assets/pineappleThree.bmp");
    pineappleFrames[3].load_bmp("assets/pineappleFour.bmp");

    SDL_Log("Loading apple frames...");
    appleFrames[0].load_bmp("assets/appleOne.bmp");
    appleFrames[1].load_bmp("assets/appleTwo.bmp");
    appleFrames[2].load_bmp("assets/appleThree.bmp");
    appleFrames[3].load_bmp("assets/appleFour.bmp");

    SDL_Log("Loading blueberry frames...");
    blueberryFrames[0].load_bmp("assets/blueberryOne.bmp");
    blueberryFrames[1].load_bmp("assets/blueberryTwo.bmp");
    blueberryFrames[2].load_bmp("assets/blueberryThree.bmp");
    blueberryFrames[3].load_bmp("assets/blueberryFour.bmp");

    SDL_Log("Loading strawberry frames...");
    strawberryFrames[0].load_bmp("assets/strawberryOne.bmp");
    strawberryFrames[1].load_bmp("assets/strawberryTwo.bmp");
    strawberryFrames[2].load_bmp("assets/strawberryThree.bmp");
    strawberryFrames[3].load_bmp("assets/strawberryFour.bmp");

    SDL_Log("All assets loaded!");
}
