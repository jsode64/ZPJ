#include "world.h"
#include "assets.h"
#include "player.h"
#include "util.h"

World::World()
    : tiles{{}}, numTiles{0}, coins{}, numCoins{0}, fruits{{}}, numFruits{0}, dashUpgrade{}, doubleJumpUpgrade{} {}

World::~World() {}

std::span<const Tile> World::get_tiles() const { return std::span(tiles.data(), numTiles); }

void World::init(const Player& player) {
    numTiles = 0;
    numCoins = 0;
    numFruits = 0;
    i32 it = 0;

    push_tile({-2000.0f, 100.0f, 4000.0f, 50.0f});
    push_tile({-2000.0f, -1500.0f, 4000.0f, 50.0f});
    push_tile({-2050.0f, -1500.0f, 50.0f, 1650.0f});
    push_tile({2000.0f, -1500.0f, 50.0f, 1650.0f});

    push_tile({-50.0f, -75.0f, 150.0f, 50.0f});
    push_tile({100.0f, -175.0f, 50.0f, 150.0f});

    push_tile({-150.0f, 75.0f, 75.0f, 25.0f});
    push_tile({-250.0f, 50.0f, 100.0f, 50.0f});
    push_tile({-550.0f, 25.0f, 300.0f, 75.0f});

    push_tile({-650.0f, 25.0f, 100.0f, 25.0f}, TILE_CYCLE(-650.0f, 25.0f, -650.0f, -275.0f, 3.0f));
    push_tile({-650.0f, 50.0f, 100.0f, 50.0f});
    push_tile({-850.0f, 25.0f, 200.0f, 75.0f});

    push_tile({-1250.0f, -275.0f, 100.0f, 25.0f}, TILE_CYCLE(-1250.0f, -275.0f, -850.0f, -275.0f, 3.0f));
    push_tile({-1600.0f, -275.0f, 250.0f, 75.0f});

    push_tile({-1650.0f, -550.0f, 50.0f, 550.0f});
    push_tile({-2000.0f, -550.0f, 350.0f, 50.0f});
    push_tile({-2000.0f, 75.0f, 50.0f, 25.0f});
    push_tile({-1890.0f, 40.0f, 25.0f, 25.0f});
    push_tile({-1790.0f, 5.0f, 25.0f, 25.0f});
    push_tile({-1700.0f, -25.0f, 50.0f, 25.0f});
    push_tile({-1800.0f, -55.0f, 50.0f, 25.0f}, TILE_CYCLE(-1800.0f, -55.0f, -2000.0f, -55.0f, 2.0f));
    push_tile({-1700.0f, -85.0f, 50.0f, 25.0f}, TILE_CYCLE(-1700.0f, -85.0f, -2000.0f, -85.0f, 3.0f));
    push_tile({-1700.0f, -115.0f, 50.0f, 25.0f}, TILE_CYCLE(-1700.0f, -115.0f, -2000.0f, -115.0f, 1.75f));
    push_tile({-1750.0f, -150.0f, 50.0f, 25.0f}, TILE_CYCLE(-1750.0f, -150.0f, -1950.0f, -150.0f, 2.5f));
    push_tile({-1950.0f, -250.0f, 50.0f, 25.0f}, TILE_CYCLE(-1950.0f, -250.0f, -1750.0f, -250.0f, 2.5f));
    push_tile({-2000.0f, -150.0f, 50.0f, 25.0f}, TILE_CYCLE(-2000.0f, -150.0f, -2000.0f, -350.0f, 2.0f));
    push_tile({-1700.0f, -150.0f, 50.0f, 25.0f}, TILE_CYCLE(-1700.0f, -150.0f, -1700.0f, -350.0f, 2.0f));
    push_tile({-1935.0f, -400.0f, 225.0f, 25.0f});
    push_tile({-1835.0f, -500.0f, 25.0f, 125.0f});

    push_tile({250.0f, 50.0f, 100.0f, 50.0f});
    push_tile({450.0f, 0.0f, 100.0f, 100.0f});
    push_tile({650.0f, -50.0f, 100.0f, 150.0f});
    push_tile({750.0f, 0.0f, 50.0f, 100.0f});
    push_tile({800.0f, 50.0f, 50.0f, 50.0f});

    push_tile({300.0f, -200.0f, 50.0f, 50.0f});
    push_tile({500.0f, -225.0f, 50.0f, 50.0f});
    push_tile({700.0f, -255.0f, 50.0f, 50.0f});
    push_tile({900.0f, -300.0f, 200.0f, 50.0f});

    push_tile({1200.0f, -300.0f, 50.0f, 50.0f}, TILE_CYCLE(1200.0f, -300.0f, 1200.0f, -500.0f, 2.0f));

    push_tile({-300.0f, -175.0f, 150.0f, 50.0f});
    push_tile({-350.0f, -400.0f, 50.0f, 275.0f});

    push_tile({-150.0f, -425.0f, 250.0f, 50.0f});
    push_tile({225.0f, -425.0f, 50.0f, 50.0f}, TILE_CYCLE(225.0f, -425.0f, 425.0f, -625.0f, 2.0f));
    push_tile({350.0f, -825.0f, 50.0f, 50.0f}, TILE_CYCLE(350.0f, -825.0f, 550.0f, -625.0f, 2.0f));
    push_tile({-50.0f, -825.0f, 250.0f, 50.0f});

    // Coins
    push_coin({100.0f, 65.0f});
    push_coin({150.0f, 65.0f});
    push_coin({200.0f, 65.0f});

    // Fruits
    push_fruit(Fruit(-200.0f, 150.0f, FruitType::Orange));
    push_fruit(Fruit(-150.0f, 150.0f, FruitType::Pineapple));
    push_fruit(Fruit(-100.0f, 150.0f, FruitType::Apple));
    push_fruit(Fruit(-50.0f, 150.0f, FruitType::Blueberry));
    push_fruit(Fruit(0.0f, 150.0f, FruitType::Strawberry));

    for (i32 i = 0; i < 5; i++) {
        push_coin({-510.0f + (i * 50.0f), -10.0f});
    }

    it = 1;
    for (i32 i = 0; i < 5; i++) {
        for (i32 j = 0; j < it; j++) {
            push_coin({-1480.0f + (i * -15.0f) + (j * 30.0f), -430.0f + (i * 30.0f)});
        }
        it++;
    }

    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 3; j++) {
            push_coin({-1800.0f + (i * 20.0f), -485.0f + (j * 25.0f)});
        }
    }

    push_coin({300.0f, 15.0f});

    doubleJumpUpgrade = Upgrade::double_jump(-1900.0f, -460.0f, !player.has_double_jump_unlocked());
}

void World::update(Player& player) {
    const auto playerBody = player.get_body();

    // Update tiles.
    for (auto& tile : std::span(tiles.data(), numTiles)) {
        tile.update();
    }

    // Check for collected coins.
    for (auto& coin : std::span(coins.data(), numCoins)) {
        const SDL_FRect coinBody{coin.get_x(), coin.get_y(), Coin::W, Coin::H};
        if (coin.is_active() && do_rects_collide(playerBody, coinBody)) {
            player.give_coins(1);
            coin.collect();
        }
    }

    // Check for collected fruits.
    for (auto& fruit : std::span(fruits.data(), numFruits)) {
        fruit.update();
        if (fruit.is_active() && do_rects_collide(playerBody, fruit.get_body())) {
            player.give_coins(5);
            fruit.collect();
            gAssets.fruitPanel.collect(fruit.get_slot());
        }
    }

    // Check for collected upgrades.
    if (doubleJumpUpgrade.is_active() && do_rects_collide(playerBody, doubleJumpUpgrade.get_body())) {
        doubleJumpUpgrade.collect(player);
    }
    if (dashUpgrade.is_active() && do_rects_collide(playerBody, dashUpgrade.get_body())) {
        dashUpgrade.collect(player);
    }
}

void World::draw(const Player& player) const {
    const auto renderer = gWindow.get_renderer();
    const SDL_FRect playerBody = player.get_body();
    const f32 winW = f32(gWindow.get_width());
    const f32 winH = f32(gWindow.get_height());

    const SDL_FPoint playerCenter(playerBody.x + (playerBody.w / 2.0f), playerBody.y + (playerBody.h / 2.0f));
    const SDL_FRect view(playerCenter.x - (winW / 2.0f), playerCenter.y - (winH / 2.0f), winW, winH);

    // Draw tiles.
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (const auto& tile : std::span(tiles.data(), numTiles)) {
        const auto body = tile.get_body();
        if (!do_rects_collide(view, body))
            continue;
        const SDL_FRect dst(body.x - view.x, body.y - view.y, body.w, body.h);
        SDL_RenderFillRect(renderer, &dst);
    }

    // Draw coins.
    SDL_FRect coinBody({}, {}, Coin::W, Coin::H);
    SDL_SetRenderDrawColor(renderer, 255, 225, 50, 255);
    for (const auto& coin : std::span(coins.data(), numCoins)) {
        coinBody.x = coin.get_x();
        coinBody.y = coin.get_y();
        if (!coin.is_active() || !do_rects_collide(view, coinBody))
            continue;
        const SDL_FRect dst(coinBody.x - view.x, coinBody.y - view.y, coinBody.w, coinBody.h);
        SDL_RenderFillRect(renderer, &dst);
    }

    // Draw fruits.
    for (const auto& fruit : std::span(fruits.data(), numFruits)) {
        const auto fruitBody = fruit.get_body();
        if (!fruit.is_active() || !do_rects_collide(view, fruitBody))
            continue;
        const SDL_FRect dst{fruitBody.x - view.x, fruitBody.y - view.y + fruit.get_float_offset(), Fruit::W, Fruit::H};
        SDL_RenderTexture(renderer, gAssets.get_fruit_frames(fruit.get_type())[fruit.get_frame()].get(), nullptr, &dst);
    }

    // Draw upgrades.
    const auto doubleJumpBody = doubleJumpUpgrade.get_body();
    if (doubleJumpUpgrade.is_active() && do_rects_collide(view, doubleJumpBody)) {
        const SDL_FRect dst{doubleJumpBody.x - view.x, doubleJumpBody.y - view.y, doubleJumpBody.w, doubleJumpBody.h};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &dst);
    }
    const auto dashBody = dashUpgrade.get_body();
    if (dashUpgrade.is_active() && do_rects_collide(view, dashBody)) {
        const SDL_FRect dst{dashBody.x - view.x, dashBody.y - view.y, dashBody.w, dashBody.h};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &dst);
    }
}