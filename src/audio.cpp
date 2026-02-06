#include "audio.h"

#include <exception>
#include <format>
#include <SDL3/SDL.h>

Audio gAudio;

Audio::Audio() : stream{nullptr} {

}

Audio::~Audio() {
    if (stream) {
        SDL_DestroyAudioStream(stream);
    }
}

void Audio::play_sound(const Sound& sound) {
    SDL_ClearAudioStream(stream);
    SDL_PutAudioStreamData(stream, sound.get_data(), sound.get_length());
}

void Audio::init() {
    if (!SDL_Init(SDL_INIT_AUDIO)) {
        const auto e = std::format("Failed to initialize SDL audio: \"{}\"", SDL_GetError());
        throw std::runtime_error(e);
    }
    
    // Gotten from `assets/coin.wav`.
    const SDL_AudioSpec spec = {
        .channels = 2,
        .format = SDL_AUDIO_S16,
        .freq = 44'100,
    };

    stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec,
        nullptr,
        nullptr
    );

    if (!stream) {
        const auto e = std::format("Failed to load SDL audio stream: \"{}\"", SDL_GetError());
        throw std::runtime_error(e);
    }

    SDL_ResumeAudioStreamDevice(stream);
}
