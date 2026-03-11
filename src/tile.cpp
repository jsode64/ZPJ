#include "tile.h"

Tile::UpdateFn Tile::get_update_fn() const { return updateFn; }

SDL_FRect Tile::get_body() const { return body; }

SDL_FPoint Tile::get_v() const { return v; }

void Tile::set_v(const SDL_FPoint _v) { v = _v; }

void Tile::update() {
    if (updateFn) {
        updateFn(*this);
    }

    body.x += v.x;
    body.y += v.y;
}
