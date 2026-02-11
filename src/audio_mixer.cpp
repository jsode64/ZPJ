#include "audio_mixer.h"
#include <SDL3/SDL.h>

//https://github.com/libsdl-org/SDL_mixer/blob/main/docs/hello.c

AudioMixer g_audio_mixer;

AudioMixer::AudioMixer() : mixer(nullptr), coinTrack(nullptr), jumpTrack(nullptr), coinSound(nullptr), jumpSound(nullptr) {
}

AudioMixer::~AudioMixer() {
    shutdown();
}

bool AudioMixer::init() {
    if (!MIX_Init()) {
        SDL_Log("Couldn't initialize SDL_mixer: %s", SDL_GetError());
        return false;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        SDL_Log("Couldn't create mixer: %s", SDL_GetError());
        return false;
    }

    // load coin sound
    coinSound = MIX_LoadAudio(mixer, "assets/coin.wav", true);
    if (!coinSound) {
        SDL_Log("Couldn't load coin.wav: %s", SDL_GetError());
        return false;
    }

    // load jump sound
    jumpSound = MIX_LoadAudio(mixer, "assets/jump.mp3", true);
    if (!jumpSound) {
        SDL_Log("Couldn't load jump.mp3: %s", SDL_GetError());
        return false;
    }

    // separate tracks for coin and jump
    coinTrack = MIX_CreateTrack(mixer);
    if (!coinTrack) {
        SDL_Log("Couldn't create coin track: %s", SDL_GetError());
        return false;
    }

    jumpTrack = MIX_CreateTrack(mixer);
    if (!jumpTrack) {
        SDL_Log("Couldn't create jump track: %s", SDL_GetError());
        return false;
    }

    return true;
}

void AudioMixer::shutdown() {
    MIX_Quit();
    mixer = nullptr;
    coinTrack = nullptr;
    jumpTrack = nullptr;
    coinSound = nullptr;
    jumpSound = nullptr;
}


// functions to call to play sounds
void AudioMixer::play_coin_sound() {
    if (coinTrack && coinSound) {
        MIX_SetTrackAudio(coinTrack, coinSound);
        MIX_PlayTrack(coinTrack, 0);
    }
}

void AudioMixer::play_jump_sound() {
    if (jumpTrack && jumpSound) {
        MIX_SetTrackAudio(jumpTrack, jumpSound);
        MIX_PlayTrack(jumpTrack, 0);
    }
}