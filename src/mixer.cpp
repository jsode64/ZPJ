#include "mixer.h"

#include <SDL3/SDL.h>
#include <exception>
#include <format>

// `https://github.com/libsdl-org/SDL_mixer/blob/main/docs/hello.c`

Mixer gMixer;

Mixer::Mixer() : mixer{nullptr} {}

Mixer::~Mixer() { MIX_Quit(); }

MIX_Mixer* Mixer::get_mixer() { return mixer; }

void Mixer::play_sound(Sound& sound) {
  const auto [audio, track] = sound.get_data();
  MIX_PlayTrack(track, 0);
}

void Mixer::init() {
  if (!MIX_Init() || !(mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr))) {
    const auto e = std::format("Failed to init SDL mixer: \"{}\"", SDL_GetError());
  }
}