#include "tile.h"

Tile::UpdateFn Tile::get_update_fn() const { return updateFn; }

SDL_FRect Tile::get_body() const { return body; }

void Tile::update() {
    if (updateFn) {
        *this = updateFn(*this);
    }
}
