#pragma once

#include <tuple>
#include <SDL3_mixer/SDL_mixer.h>

#include "def.h"

class Sound {
private:
    /** @brief The sound data. */
    MIX_Audio* audio;

    /** @brief The sound's track. */
    MIX_Track* track;

public:
    constexpr Sound() : audio{nullptr}, track{nullptr} {

    }

    Sound(const char* path);

    ~Sound();

    /** @brief Returns the audio and track. */
    std::tuple<MIX_Audio*, MIX_Track*> get_data();

    /** @brief Loads the given audio file into the sound. */
    void load(const char* path);
};
