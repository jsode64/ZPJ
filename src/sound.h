#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <tuple>

#include "def.h"

class Sound {
private:
  /** The sound data. */
  MIX_Audio* audio;

  /** The sound's track. */
  MIX_Track* track;

public:
  constexpr Sound() : audio{nullptr}, track{nullptr} {}

  Sound(const char* path);

  ~Sound();

  /** Returns the audio and track. */
  std::tuple<MIX_Audio*, MIX_Track*> get_data();

  /** Loads the given audio file into the sound. */
  void load(const char* path);
};
