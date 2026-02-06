#include "sound.h"

#include <exception>
#include <format>

Sound::Sound(const char* path) : Sound{} {
    SDL_LoadWAV(path, &spec, &data, &length);
}

Sound::~Sound() {
    SDL_free(data);
}

const u8* Sound::get_data() const {
    return data;
}

SDL_AudioSpec Sound::get_spec() const {
    return spec;
}

u32 Sound::get_length() const {
    return length;
}

void Sound::load_wav(const char* path) {
    if (data) {
        SDL_free(data);
    }

    if (!SDL_LoadWAV(path, &spec, &data, &length)) {
        const auto e = std::format("Failed to load `.wav` file: \"{}\"", SDL_GetError());
        throw std::runtime_error(e);
    }
}
