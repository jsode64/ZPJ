#include "world.h"

#include "player.h"
#include "util.h"

World::World() : tiles{{}}, numTiles{0}, coins{}, numCoins{0}, dashUpgrade{}, doubleJumpUpgrade{} {}

World::~World() {}

std::span<const Tile> World::get_tiles() const { return std::span(tiles.data(), numTiles); }

void World::init(const Player& player) {
    numTiles = 0;
    numCoins = 0;

  // Tiles for the edge of the world
  push_tile({-2000.0f, 100.0f, 4000.0f, 50.0f});
  push_tile({-2000.0f, -1500.0f, 4000.0f, 50.0f});
  push_tile({-2050.0f, -1500.0f, 50.0f, 1650.0f});
  push_tile({2000.0f, -1500.0f, 50.0f, 1650.0f});

  // Stage tiles
  //
  push_tile({-50.0f, -75.0f, 150.0f, 50.0f});
  push_tile({100.0f, -175.0f, 50.0f, 150.0f});

  // Staircase to the left
  push_tile({-150.0f, 75.0f, 75.0f, 25.0f});
  push_tile({-250.0f, 50.0f, 100.0f, 50.0f});
  push_tile({-550.0f, 25.0f, 300.0f, 75.0f});

  // Staircase to the right
  push_tile({250.0f, 50.0f, 100.0f, 50.0f});
  push_tile({450.0f, 0.0f, 100.0f, 100.0f});
  push_tile({650.0f, -50.0f, 100.0f, 150.0f});
  push_tile({750.0f, 0.0f, 50.0f, 100.0f});
  push_tile({800.0f, 50.0f, 50.0f, 50.0f});

  push_tile({300.0f, -200.0f, 50.0f, 50.0f});
  push_tile({500.0f, -225.0f, 50.0f, 50.0f});
  push_tile({700.0f, -255.0f, 50.0f, 50.0f});
  push_tile({900.0f, -300.0f, 200.0f, 50.0f});
  push_tile({-300.0f, -175.0f, 150.0f, 50.0f});
  push_tile({-350.0f, -400.0f, 50.0f, 275.0f});

  // Coins to the right
  push_coin({100.0f, 65.0f});
  push_coin({150.0f, 65.0f});
  push_coin({200.0f, 65.0f});

  // A line of coins on platform to the left
  for (i32 i = 0; i < 5; i++) {
    push_coin({-510.0f + (i * 50.0f), -10.0f});
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

    doubleJumpUpgrade = Upgrade::double_jump(200.0f, 0.0f, !player.has_double_jump_unlocked());
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
        if (do_rects_collide(playerBody, coinBody)) {
            player.give_coins(1);
            coin.collect();
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

    // Draw tiles that are within the view.
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (const auto& tile : std::span(tiles.data(), numTiles)) {
        // Skip tiles that can't be seen.
        const auto body = tile.get_body();
        if (!do_rects_collide(view, body)) {
            continue;
        }

        // Draw the tile relative to the view.
        const SDL_FRect dst(body.x - view.x, body.y - view.y, body.w, body.h);
        SDL_RenderFillRect(renderer, &dst);
    }

    // Draw coins that are within the view.
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
        SDL_RenderFillRect(renderer, &dst);
    }

    // Draw upgrades.
    const auto doubleJumpBody = doubleJumpUpgrade.get_body();
    if (doubleJumpUpgrade.is_active() && do_rects_collide(view, doubleJumpBody)) {
        const SDL_FRect dst{
            doubleJumpBody.x - view.x,
            doubleJumpBody.y - view.y,
            doubleJumpBody.w,
            doubleJumpBody.h,
        };
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &dst);
    }
    const auto dahsBody = dashUpgrade.get_body();
    if (dashUpgrade.is_active() && do_rects_collide(view, dahsBody)) {
        const SDL_FRect dst{
            dahsBody.x - view.x,
            dahsBody.y - view.y,
            dahsBody.w,
            dahsBody.h,
        };
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &dst);
    }
}
