#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "fruit.h"
#include "fruitpanel.h"
#include "powerbar.h"
#include "sound.h"
#include "texture.h"

class Assets {
  public:
    /** The path to the game's font. */
    static constexpr const char* FONT_PATH = "assets/comic_sans.ttf";

    /** The game's font. */
    TTF_Font* font;

    /** The jump sound. */
    Sound jumpSound;

    /** The coin collect sound. */
    Sound coinCollectSound;

    /** The menu text. */
    Texture menuText;

    /** The power bar. */
    PowerBar powerBar;

    /** The fruit panel. */
    FruitPanel fruitPanel;

    /** Fruit animation frames per type. */
    Texture orangeFrames[4];
    Texture pineappleFrames[4];
    Texture appleFrames[4];
    Texture blueberryFrames[4];
    Texture strawberryFrames[4];

    /** Returns the correct frame array for the given fruit type. */
    Texture* get_fruit_frames(FruitType type) {
        switch (type) {
        case FruitType::Orange:
            return orangeFrames;
        case FruitType::Pineapple:
            return pineappleFrames;
        case FruitType::Apple:
            return appleFrames;
        case FruitType::Blueberry:
            return blueberryFrames;
        case FruitType::Strawberry:
            return strawberryFrames;
        }
        return orangeFrames;
    }

    Assets() : font{nullptr}, jumpSound{}, coinCollectSound{}, menuText{}, powerBar{}, fruitPanel{} {}

    /** Loads the game assets. */
    void load_assets();
};

/** The global asset storage. */
extern Assets gAssets;