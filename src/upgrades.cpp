#include "upgrades.h"

#include "player.h"

Upgrade::Upgrade() : body{}, cb{}, isActive{false} {}

Upgrade::Upgrade(const SDL_FRect body, const UpgradeCollectCb cb, const bool shouldActivate)
    : body{body}, cb{cb}, isActive{shouldActivate} {}

Upgrade Upgrade::dash_upgrade(const f32 x, const f32 y, bool shouldActivate) {
    return Upgrade(SDL_FRect{x, y, W, H}, [](Player& player) { player.unlock_dash(); }, shouldActivate);
}

Upgrade Upgrade::double_jump(const f32 x, const f32 y, bool shouldActivate) {
    return Upgrade(SDL_FRect{x, y, W, H}, [](Player& player) { player.unlock_double_jump(); }, shouldActivate);
}

SDL_FRect Upgrade::get_body() const { return body; }

bool Upgrade::is_active() const { return isActive; }

void Upgrade::collect(Player& player) {
    if (cb) {
        cb(player);
    }
    isActive = false;
}
