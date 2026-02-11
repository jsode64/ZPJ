#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include "sound.h"

class Mixer {
private:
    /** @brief The mixer device for audio playback. */
    MIX_Mixer* mixer;

public:
    Mixer();
    
    ~Mixer();

    /** @brief Returns the mixer. */
    MIX_Mixer* get_mixer();

    /** @brief Plays the given sound. */
    void play_sound(Sound& sound);

    /** @brief Initializes the audio mixer. */
    void init();
};

/** @brief Global audio mixer instance. */
extern Mixer gMixer;
