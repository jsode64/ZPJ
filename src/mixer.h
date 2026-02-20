#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include "sound.h"

class Mixer {
private:
  /** The mixer device for audio playback. */
  MIX_Mixer* mixer;

public:
  Mixer();

  ~Mixer();

  /** Returns the mixer. */
  MIX_Mixer* get_mixer();

  /** Plays the given sound. */
  void play_sound(Sound& sound);

  /** Initializes the audio mixer. */
  void init();
};

/** Global audio mixer instance. */
extern Mixer gMixer;
