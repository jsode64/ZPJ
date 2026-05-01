#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "sound.h"
#include "texture.h"

class Assets {
  public:
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

    /** The rock texture for platforms. */
    Texture rock;

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
