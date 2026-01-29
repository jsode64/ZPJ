#include "world.h"

#include "util.h"

World::World() : tiles({SDL_FRect(0.0f, 0.0f, 0.0f, 0.0f)}), numTiles(0) {
    push_tile(SDL_FRect(-100.0f, 100.0f, 200.0f, 50.0f));
    push_tile(SDL_FRect(-150.0f, 50.0f, 75.0f, 50.0f));
}

World::~World() {

}

std::span<const SDL_FRect> World::get_tiles() const {
    return std::span<const SDL_FRect>(tiles.data(), numTiles);
}

void World::draw(Window& window, const Player& player) const {
    const SDL_FRect body = player.get_body();
    const f32 winW = f32(window.get_width());
    const f32 winH = f32(window.get_height());
    auto renderer = window.get_renderer();

    // Calculate the camera's view.
    const SDL_FPoint playerCenter(
        body.x + (body.w / 2.0f),
        body.y + (body.h / 2.0f)
    );
    const SDL_FRect view(
        playerCenter.x - (winW / 2.0f),
        playerCenter.y - (winH / 2.0f),
        winW,
        winH
    );

    // Draw tiles that are within the view.
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (const auto& tile : get_tiles()) {
        // Skip tiles that can't be seen.
        if (!do_rects_collide(view, tile)) {
            continue;
        }

        // Draw the tile relative to the player's center.
        const SDL_FRect dst(
            tile.x - view.x,
            tile.y - view.y,
            tile.w,
            tile.h
        );
        SDL_RenderFillRect(renderer, &dst);
    }
}
