#pragma once

#include <SDL3/SDL.h>

#include "def.h"
#include "sound.h"
class Audio {
private:
    /** @brief The audio stream for playback. */
    SDL_AudioStream* stream;

public:
    Audio();

    ~Audio();

    /** @brief Plays the given sound. */
    void play_sound(const Sound& sound);

    /** @brief Initializes the audio stream. */
    void init();
};

/** @brief Global audio instance. */
extern Audio gAudio;
