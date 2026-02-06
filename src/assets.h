#pragma once

#include <SDL3/SDL.h>

#include "sound.h"

class Assets {
public:
    /** @brief The coin collect sound. */
    Sound coinCollectSound;

    Assets();

    /** @brief Loads the game assets. */
    bool load_assets();
};

/** @brief The global asset storage. */
extern Assets gAssets;
