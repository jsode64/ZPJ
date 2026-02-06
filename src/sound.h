#pragma once

#include <SDL3/SDL.h>

#include "def.h"

class Sound {
private:
    /** @brief The sound's data. */
    u8* data;

    /** @brief The sound's audio spec. */
    SDL_AudioSpec spec;

    /** @brief The sound's length. */
    u32 length;

public:
    constexpr Sound() : data{nullptr}, spec{}, length{0} {

    }

    Sound(const char* path);

    ~Sound();

    /** @brief Returns a pointer to the sound's data. */
    const u8* get_data() const;

    /** @brief Returns the sound's audio spec. */
    SDL_AudioSpec get_spec() const;

    /** @brief Returns the sound's length. */
    u32 get_length() const;

    /** @brief Loads the given `.wav` file into the sound. */
    void load_wav(const char* path);
};
