#pragma once

#include <SDL3_mixer/SDL_mixer.h>


class AudioMixer {
private:
    /** @brief The mixer device for audio playback. */
    MIX_Mixer* mixer;

    /** @brief Track for playing coin sounds. */
    MIX_Track* coinTrack;

    /** @brief Track for playing jump sounds. */
    MIX_Track* jumpTrack;

    /** @brief The loaded coin sound effect. */
    MIX_Audio* coinSound;

    /** @brief The loaded jump sound effect. */
    MIX_Audio* jumpSound;

public:
    AudioMixer();
    ~AudioMixer();

    /** @brief Initialize the audio mixer and load assets. */
    bool init();

    /** @brief Shutdown the audio mixer. */
    void shutdown();

    /** @brief Plays the coin collection sound. */
    void play_coin_sound();

    /** @brief Plays the jump sound. */
    void play_jump_sound();
};

/** @brief Global audio mixer instance. */
extern AudioMixer g_audio_mixer;
