#include "sound.h"

#include "mixer.h"

#include <exception>
#include <format>

Sound::Sound(const char* path) : Sound{} {
    load(path);
}

Sound::~Sound() {
    MIX_DestroyAudio(audio);
    MIX_DestroyTrack(track);
}

std::tuple<MIX_Audio*, MIX_Track*> Sound::get_data() {
    return std::tuple(audio, track);
}

void Sound::load(const char* path) {
    // Prevent overwrites.
    MIX_DestroyAudio(audio);
    MIX_DestroyTrack(track);

    const auto mixer = gMixer.get_mixer();

    // Load the sound.
    if (!(audio = MIX_LoadAudio(mixer, path, true))) {
        const auto e = std::format("Failed to load audio file `{}`: \"{}\"", path, SDL_GetError());
        throw std::runtime_error(e);
    }

    // Create the track.
    if (!(track = MIX_CreateTrack(mixer))) {
        const auto e = std::format("Failed to create mixer track: \"{}\"", SDL_GetError());
        throw std::runtime_error(e);
    }

    MIX_SetTrackAudio(track, audio);
}
