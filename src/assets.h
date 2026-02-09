#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "sound.h"
#include "texture.h"

class Assets {
public:
    /** @brief The path to the game's font. */
    static constexpr const char* FONT_PATH = "assets/comic_sans.ttf";

    /** @brief The game's font. */
    TTF_Font* font;

    /** @brief The coin collect sound. */
    Sound coinCollectSound;

    /** @brief The menu text. */
    Texture menuText;

    Assets();

    /** @brief Loads the game assets. */
    void load_assets();
};

/** @brief The global asset storage. */
extern Assets gAssets;
