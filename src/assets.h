#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

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

    /** The player sprite map. */
    Texture player;

    /** The coin sprite map. */
    Texture coin;

    /** The grass texture for platforms. */
    Texture grass;

    /** The double jump upgrade texture. */
    Texture doubleJump;

    /** The dash upgrade texture. */
    Texture dash;

    /** The fruit collectable texture. */
    Texture fruit;

    constexpr Assets() : font{nullptr}, jumpSound{}, coinCollectSound{}, menuText{} {}

    /** Loads the game assets. */
    void load_assets();
};

/** The global asset storage. */
extern Assets gAssets;
