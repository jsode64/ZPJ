#include "audio.h"

#include <SDL3/SDL.h>

Audio g_audio;

Audio::Audio() : stream(nullptr), coinSoundData(nullptr), coinSoundLength(0), coinSpec{} {

}

Audio::~Audio() {
    if (stream) {
        SDL_DestroyAudioStream(stream);
    }
    if (coinSoundData) {
        SDL_free(coinSoundData);
    }
}

void Audio::load_assets() {
    // load coin sound
    if (!SDL_LoadWAV("assets/coin.wav", &coinSpec, &coinSoundData, &coinSoundLength)) {
        SDL_Log("Failed to load coin.wav: %s", SDL_GetError());
        return;
    }

    // audio stream
    stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &coinSpec,
        nullptr,
        nullptr
    );

    if (!stream) {
        SDL_Log("Failed to create audio stream: %s", SDL_GetError());
        return;
    }
    SDL_ResumeAudioStreamDevice(stream);
}

void Audio::play_coin_sound() {
    if (stream && coinSoundData) {
        // clear pending audio, queue coin sound
        SDL_ClearAudioStream(stream);
        SDL_PutAudioStreamData(stream, coinSoundData, coinSoundLength);
    }
}
