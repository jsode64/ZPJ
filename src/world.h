#pragma once

#include "coin.h"
#include "def.h"
#include "fruit.h"
#include "tile.h"
#include "upgrades.h"
#include "window.h"
#include <SDL3/SDL.h>
#include <array>
#include <cassert>
#include <span>

class Player;

class World {
private:
    /** The maximum number of objects that can be in a world. */
    static constexpr usize MAX_NUM_OBJS = 1024;

    /** The world's tiles. */
    std::array<Tile, MAX_NUM_OBJS> tiles;

    /** The number of tiles in the world. */
    usize numTiles;

    /** The world's coins. */
    std::array<Coin, MAX_NUM_OBJS> coins;

    /** The number of coins in the world. */
    usize numCoins;

    /** The world's fruit. */
    std::array<Fruit, 5> fruits;

    /** The dash upgrade. */
    Upgrade dashUpgrade;

    /** The double jump upgrade. */
    Upgrade doubleJumpUpgrade;

    /** Asserts that there's room for the tile and pushes it to the world. */
    constexpr void push_tile(const SDL_FRect body, Tile::UpdateFn updateFn = nullptr) {
        assert(numTiles < MAX_NUM_OBJS);

        tiles[numTiles] = Tile(body, updateFn);
        numTiles++;
    }

    /** Asserts that there's room for the coin and pushes it to the world. */
    constexpr void push_coin(const Coin coin) {
        assert(numCoins < MAX_NUM_OBJS);

        coins[numCoins] = coin;
        numCoins++;
    }

public:
    /** The exit door. */
    static constexpr SDL_FRect DOOR{0, 0, 45, 80};

    World();

    ~World();

    /** Returns a span of all tiles. */
    std::span<const Tile> get_tiles() const;

    /** Returns whether the fruits are all collected. */
    bool are_fruits_collected() const;

    /** Initializes the world. */
    void init(const Player& player);

    /** Updates the world. */
    void update(Player& player);

    /** Draws the stage from the given player's PoV. */
    void draw(const Player& player) const;
};
