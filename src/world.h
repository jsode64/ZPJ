#pragma once

#include <array>
#include <cassert>
#include <span>
#include <SDL3/SDL.h>

#include "def.h"
#include "player.h"
#include "window.h"

class Player;

class World {
private:
    /** @brief The maximum number of tiles that can be in a world. */
    static constexpr usize MAX_NUM_TILES = 64;

    /** @brief The world's tiles. */
    std::array<SDL_FRect, MAX_NUM_TILES> tiles;

    /** @brief The number of tiles in the world. */
    usize numTiles;

    /** @brief Asserts that there's room for the tile and pushes it to the world. */
    constexpr void push_tile(SDL_FRect tile) {
        assert(numTiles < MAX_NUM_TILES);

        tiles[numTiles] = tile;
        numTiles++;
    }

public:
    World();

    ~World();

    /** @brief Returns a view of the world's tiles. */
    std::span<const SDL_FRect> get_tiles() const;

    /** @brief Draws the stage from the given player's PoV. */
    void draw(Window& window, const Player& player) const;
};
