#include "world.h"
#include <random>
#include "assets.h"
#include "player.h"
#include "util.h"

World::World() : tiles{{}}, numTiles{0}, coins{}, numCoins{0}, fruits{}, dashUpgrade{}, doubleJumpUpgrade{} {}

World::~World() {}

std::span<const Tile> World::get_tiles() const { return std::span(tiles.data(), numTiles); }

bool World::are_fruits_collected() const {
    for (const auto& fruit: fruits) {
        if (fruit.is_active()) {
            return false;
        }
    }
    return true;
}

void World::init(const Player& player) {
    numTiles = 0;
    numCoins = 0;
    // Damage tiles for the true edge of the world
    // (in case the player clips out of the level)
    push_damage_tile({-3000.0f, 600.0f, 6000.0f, 50.0f});
    push_damage_tile({-3000.0f, -2000.0f, 6000.0f, 50.0f});
    push_damage_tile({-3050.0f, -2000.0f, 50.0f, 2650.0f});
    push_damage_tile({3000.0f, -2000.0f, 50.0f, 2650.0f});

    // Tiles for the edge of the world
    push_tile({-2000.0f, 100.0f, 4000.0f, 50.0f});
    push_tile({-2000.0f, -1500.0f, 4000.0f, 50.0f});
    push_tile({-2050.0f, -1500.0f, 50.0f, 1650.0f});
    push_tile({2000.0f, -1500.0f, 50.0f, 1650.0f});

    // Stage tiles
    // it's those tiles!!
    push_tile({-50.0f, -75.0f, 150.0f, 50.0f});
    push_tile({100.0f, -175.0f, 50.0f, 150.0f});

    // Staircase to the left
    push_tile({-150.0f, 75.0f, 75.0f, 75.0f});
    push_tile({-250.0f, 50.0f, 100.0f, 100.0f});
    push_tile({-550.0f, 25.0f, 300.0f, 125.0f});

    push_tile({-650.0f, 25.0f, 100.0f, 25.0f}, TILE_CYCLE(-650.0f, 25.0f, -650.0f, -275.0f, 3.0f));
    push_tile({-650.0f, 50.0f, 100.0f, 100.0f});
    push_tile({-850.0f, 25.0f, 200.0f, 125.0f});

    push_tile({-1250.0f, -275.0f, 100.0f, 25.0f}, TILE_CYCLE(-1250.0f, -275.0f, -850.0f, -275.0f, 3.0f));
    push_tile({-1600.0f, -275.0f, 250.0f, 75.0f});

    // Platforming challenge to the far left for the double jump upgrade
    push_tile({-1650.0f, -550.0f, 50.0f, 550.0f});
    push_tile({-2000.0f, -550.0f, 350.0f, 50.0f});
    push_tile({-2000.0f, 75.0f, 50.0f, 75.0f});
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

    // Staircase to the right
    push_tile({250.0f, 50.0f, 100.0f, 100.0f});
    push_tile({450.0f, 0.0f, 100.0f, 150.0f});
    push_tile({650.0f, -50.0f, 100.0f, 200.0f});
    push_tile({750.0f, 0.0f, 50.0f, 150.0f});
    push_tile({800.0f, 50.0f, 50.0f, 100.0f});
    push_damage_tile({850.0f, 85.0f, 1150.0f, 15.0f});

    // Platforming challenge to the far right down
    push_tile({925.0f, 25.0f, 100.0f, 25.0f}, TILE_CYCLE(925.0f, 25.0f, 1325.0f, 25.0f, 5.0f));
    push_tile({1875.0f, 25.0f, 100.0f, 25.0f}, TILE_CYCLE(1875.0f, 25.0f, 1450.0f, 25.0f, 5.0f));
    push_damage_tile({1050.0f, -130.0f, 25.0f, 50.0f}, TILE_CYCLE(1050.0f, -130.0f, 1050.0f, -30.0f, 1.5f));
    push_damage_tile({1200.0f, -30.0f, 25.0f, 50.0f}, TILE_CYCLE(1200.0f, -30.0f, 1200.0f, -130.0f, 1.5f));
    push_damage_tile({1300.0f, -15.0f, 25.0f, 25.0f});
    push_damage_tile({1425.0f, -150.0f, 25.0f, 120.0f});

    // Floating box staircase to the right
    push_tile({300.0f, -200.0f, 50.0f, 50.0f});
    push_tile({500.0f, -225.0f, 50.0f, 50.0f});
    push_tile({700.0f, -255.0f, 50.0f, 50.0f});
    push_tile({900.0f, -300.0f, 200.0f, 50.0f});
    push_tile({1200.0f, -300.0f, 50.0f, 50.0f}, TILE_CYCLE(1200.0f, -300.0f, 1200.0f, -500.0f, 2.0f));
    push_tile({1600.0f, -500.0f, 50.0f, 50.0f}, TILE_CYCLE(1600.0f, -700.0f, 1325.0f, -500.0f, 2.0f));

    // Platforming challenge to the far right up for the dash upgrade (?)
    push_tile({1750.0f, -700.0f, 50.0f, 50.0f});
    push_tile({1800.0f, -675.0f, 200.0f, 25.0f});
    push_tile({1750.0f, -1450.0f, 50.0f, 600.0f});
    push_damage_tile({1800.0f, -875.0f, 100.0f, 25.0f});
    push_damage_tile({1900.0f, -1000.0f, 100.0f, 25.0f});
    push_damage_tile({1800.0f, -1325.0f, 25.0f, 225.0f});
    push_damage_tile({1825.0f, -1125.0f, 75.0f, 25.0f});
    push_damage_tile({1975.0f, -1325.0f, 25.0f, 325.0f});
    push_tile({1800.0f, -699.0f, 200.0f, 25.0f}, TILE_CYCLE(1800.0f, -699.0f, 1800.0f, -1325.0f, 6.0f));

    // The L shaped floating platform to the left
    push_tile({-300.0f, -175.0f, 150.0f, 50.0f});
    push_tile({-350.0f, -400.0f, 50.0f, 275.0f});

    // The floating platforms accessible via the L shaped platform
    push_tile({-150.0f, -425.0f, 250.0f, 50.0f});
    push_tile({225.0f, -425.0f, 50.0f, 50.0f}, TILE_CYCLE(225.0f, -425.0f, 425.0f, -625.0f, 2.0f));
    push_tile({350.0f, -825.0f, 50.0f, 50.0f}, TILE_CYCLE(350.0f, -825.0f, 550.0f, -625.0f, 2.0f));
    push_tile({-50.0f, -825.0f, 250.0f, 50.0f});

    // Coins to the right
    push_coin({100.0f, 65.0f});
    push_coin({150.0f, 65.0f});
    push_coin({200.0f, 65.0f});

    // A line of coins on platform to the left
    for (i32 i = 0; i < 5; i++) {
        push_coin({-510.0f + (i * 50.0f), -10.0f});
    }

    // A triangle of coins on the floating platform to the left
    for (i32 i = 0; i < 5; i++) {
        for (i32 j = 0; j < i + 1; j++) {
            push_coin({-1480.0f + (i * -15.0f) + (j * 30.0f), -430.0f + (i * 30.0f)});
        }
    }

    // A square of coins in the platforming challenge to the far left
    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 3; j++) {
            push_coin({-1800.0f + (i * 20.0f), -485.0f + (j * 25.0f)});
        }
    }

    push_coin({300.0f, 15.0f});

    // A grid of coins for testing
    /*
    for (i32 i = 0; i < 10; i++) {
      for (i32 j = 0; j < 2; j++) {
        push_coin({-600.0f + (i * 25.0f), 50.0f + (j * -25.0f)});
      }
    }
    */

    // Place fruit.
    fruits = {
        Fruit(350.0f, -50.0f, fruits[0].is_active()),
        Fruit(390.0f, -50.0f, fruits[1].is_active()),
        Fruit(430.0f, -50.0f, fruits[2].is_active()),
        Fruit(470.0f, -50.0f, fruits[3].is_active()),
        Fruit(510.0f, -50.0f, fruits[4].is_active()),
    };

    // Place upgrades.
    doubleJumpUpgrade = Upgrade::double_jump(-1900.0f, -460.0f, !player.has_double_jump_unlocked());
    dashUpgrade = Upgrade::dash_upgrade(300.0f, -50.0f, !player.has_dash_unlocked());
}

void World::update(Player& player) {
    const auto playerBody = player.get_body();

    // Update tiles.
    for (auto& tile : std::span(tiles.data(), numTiles)) {
        tile.update();
    }

    // Check for collected coins.
    for (auto& coin : std::span(coins.data(), numCoins)) {
        const SDL_FRect coinBody{
            coin.get_x(),
            coin.get_y(),
            Coin::W,
            Coin::H,
        };
        if (coin.is_active() && do_rects_collide(playerBody, coinBody)) {
            player.give_coins(1);
            coin.collect();
        }
    }

    // Check for collected fruit.
    for (auto& fruit: fruits) {
        const SDL_FRect fruitBody{
            fruit.get_x(),
            fruit.get_y(),
            Fruit::W,
            Fruit::H,
        };
        if (fruit.is_active() && do_rects_collide(playerBody, fruitBody)) {
            fruit.collect();
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

    // Calculate the camera's view.
    const SDL_FPoint playerCenter(playerBody.x + (playerBody.w / 2.0f), playerBody.y + (playerBody.h / 2.0f));
    const SDL_FRect view(playerCenter.x - (winW / 2.0f), playerCenter.y - (winH / 2.0f), winW, winH);

    // Draw the door.
    if (do_rects_collide(DOOR, view)) {
        const SDL_FRect dst{
            DOOR.x - view.x,
            DOOR.y - view.y,
            DOOR.w,
            DOOR.h,
        };
        SDL_SetRenderDrawColor(renderer, 75, 50, 50, 255);
        SDL_RenderFillRect(renderer, &dst);
    }

    // Draw tiles that are within the view.
    const auto grassTexture = gAssets.grass.get();
    u32 i = 0;
    for (const auto& tile : std::span(tiles.data(), numTiles)) {
        i++;

        // Skip tiles that can't be seen.
        const auto tileBody = tile.get_body();
        if (!do_rects_collide(view, tileBody)) {
            continue;
        }

        // Draw the tile relative to the view.
        const SDL_FRect dst{tileBody.x - view.x, tileBody.y - view.y, tileBody.w, tileBody.h};

        if (tile.is_damageable()) {
            // Draw damageable tiles as solid red rectangles
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);  // Dark red color
            SDL_RenderFillRect(renderer, &dst);
        } else {
            // Draw normal tiles with grass texture
            const SDL_FRect src{
                0, 
                0, 
                tileBody.w / 4.0f,
                tileBody.h / 4.0f,
            };
            SDL_RenderTexture(renderer, grassTexture, &src, &dst);
        }
    }

    // Draw coins that are within the view.
    const bool isFrame1 = gWindow.get_frames() % 60 >= 30;
    const SDL_FRect coinSrc{isFrame1 ? 0.0f : 4.0f, 0.0f, 4.0f, 12.0f};
    SDL_FRect coinBody({}, {}, Coin::W, Coin::H);
    SDL_SetRenderDrawColor(renderer, 255, 225, 50, 255);
    for (const auto& coin : std::span(coins.data(), numCoins)) {
        coinBody.x = coin.get_x();
        coinBody.y = coin.get_y();

        // Skip inactive coins or coins outside the view.
        if (!coin.is_active() || !do_rects_collide(view, coinBody)) {
            continue;
        }

        // Draw the coin relative to the view.
        const SDL_FRect dst(coinBody.x - view.x, coinBody.y - view.y, coinBody.w, coinBody.h);
        SDL_RenderTexture(renderer, gAssets.coin.get(), &coinSrc, &dst);
    }

    // Draw the fruit.
    const f32 pulse = 8.0f * std::sin(static_cast<f32>(gWindow.get_frames()) / 15.0f);
    SDL_FRect fruitBody{{}, {}, Fruit::W + pulse, Fruit::H + pulse};
    for (usize i = 0; i < fruits.size(); i++) {
        const Fruit& fruit = fruits[i];
        fruitBody.x = fruit.get_x() - (pulse / 2.0f);
        fruitBody.y = fruit.get_y() - (pulse / 2.0f);

        if (!fruit.is_active() || !do_rects_collide(view, fruitBody)) {
            continue;
        }

        const SDL_FRect src{static_cast<f32>(i * 8), 0.0f, 8.0f, 8.0f};
        const SDL_FRect dst{fruitBody.x - view.x, fruitBody.y - view.y, fruitBody.w, fruitBody.h};
        SDL_RenderTexture(renderer, gAssets.fruit.get(), &src, &dst);
    }

    // Draw upgrades.
    const f32 slide = 8.f * std::sin(static_cast<f32>(SDL_GetTicks()) / 200.f);
    const auto doubleJumpBody = doubleJumpUpgrade.get_body();
    if (doubleJumpUpgrade.is_active() && do_rects_collide(view, doubleJumpBody)) {
        const SDL_FRect dst{
            doubleJumpBody.x - view.x,
            doubleJumpBody.y - view.y + slide,
            doubleJumpBody.w,
            doubleJumpBody.h,
        };
        SDL_RenderTexture(renderer, gAssets.doubleJump.get(), nullptr, &dst);
    }
    const auto dashBody = dashUpgrade.get_body();
    if (dashUpgrade.is_active() && do_rects_collide(view, dashBody)) {
        const SDL_FRect dst{
            dashBody.x - view.x,
            dashBody.y - view.y + slide,
            dashBody.w,
            dashBody.h,
        };
        SDL_RenderTexture(renderer, gAssets.dash.get(), nullptr, &dst);
    }
}
