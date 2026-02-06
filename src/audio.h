#pragma once

#include <SDL3/SDL.h>

class Audio {
private:
    /** @brief The audio stream for playback. */
    SDL_AudioStream* stream;

    /** @brief The coin sound effect data. */
    Uint8* coinSoundData;

    /** @brief The coin sound effect length in bytes. */
    Uint32 coinSoundLength;

    /** @brief The audio spec for the coin sound. */
    SDL_AudioSpec coinSpec;

public:
    Audio();

    ~Audio();

    /** @brief Load audio assets. */
    void load_assets();

    /** @brief Plays the coin sound. */
    void play_coin_sound();
};

/** @brief Global audio instance. */
extern Audio g_audio;
