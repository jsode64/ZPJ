#pragma once

#include "def.h"
#include <SDL3/SDL.h>
#include <functional>

class Player;

using UpgradeCollectCb = std::function<void(Player& player)>;

class Upgrade {
  private:
    /** The upgrade's body. */
    SDL_FRect body;

    /** The upgrade's callback function. */
    UpgradeCollectCb cb;

    /** Is the upgrade active? */
    bool isActive;

    Upgrade(SDL_FRect body, UpgradeCollectCb cb, bool shouldActivate);

  public:
    /** An upgrade's width. */
    static constexpr f32 W = 25.0f;

    /** An upgrade's height. */
    static constexpr f32 H = 20.0f;

    Upgrade();

    /** A dash upgrade. */
    static Upgrade dash_upgrade(f32 x, f32 y, bool shouldActivate);

    /** A double jump upgrade. */
    static Upgrade double_jump(f32 x, f32 y, bool shouldActivate);

    /** The upgrade's body. */
    SDL_FRect get_body() const;

    /** Is the upgrade active? */
    bool is_active() const;

    /** Collect the upgrade. */
    void collect(Player& player);
};
