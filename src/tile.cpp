#include "tile.h"

Tile::UpdateFn Tile::get_update_fn() const { return updateFn; }

SDL_FRect Tile::get_body() const { return body; }

SDL_FPoint Tile::get_v() const { return v; }

void Tile::set_v(const SDL_FPoint _v) { v = _v; }

bool Tile::is_damageable() const { return damageable; }

void Tile::set_damageable(bool dmg) { damageable = dmg; }

void Tile::reset_damage_cooldown() const { damagableCooldown = 50; }

void Tile::update() {
    if (updateFn) {
        updateFn(*this);
    }

    body.x += v.x;
    body.y += v.y;

    if (damagableCooldown > 0) {
        damagableCooldown--;
    }
}
