#include "world.h"

#include "util.h"

World::World() : tiles{{}}, numTiles(0), coins{}, numCoins(0) { init(); }

World::~World() {}

std::span<const SDL_FRect> World::get_tiles() const {
  return std::span<const SDL_FRect>(tiles.data(), numTiles);
}

std::span<const Coin> World::get_coins() const {
  return std::span<const Coin>(coins.data(), numCoins);
}

std::span<Coin> World::get_coins_mut() { return std::span<Coin>(coins.data(), numCoins); }

void World::init() {
  numTiles = 0;
  numCoins = 0;

  // Tiles for the edge of the world
  push_tile({-2500.0f, 100.0f, 5000.0f, 50.0f});
  push_tile({-2500.0f, -1000.0f, 5000.0f, 50.0f});
  push_tile({-2550.0f, -1000.0f, 50.0f, 1150.0f});
  push_tile({2500.0f, -1000.0f, 50.0f, 1150.0f});

  // Stage tiles
  push_tile({-150.0f, 62.5f, 75.0f, 37.5f});
  push_tile({-50.0f, -75.0f, 150.0f, 50.0f});
  push_tile({100.0f, -175.0f, 50.0f, 150.0f});
  push_tile({-250.0f, 25.0f, 100.0f, 75.0f});
  push_tile({250.0f, 50.0f, 100.0f, 50.0f});
  push_tile({450.0f, 0.0f, 100.0f, 100.0f});
  push_tile({300.0f, -200.0f, 50.0f, 50.0f});
  push_tile({500.0f, -225.0f, 50.0f, 50.0f});
  push_tile({-300.0f, -175.0f, 150.0f, 50.0f});
  push_tile({-350.0f, -400.0f, 50.0f, 275.0f});

  push_coin({-225.0f, -10.0f});
  push_coin({100.0f, 50.0f});
  push_coin({300.0f, 0.0f});
}

void World::draw(const Player& player) const {
  const auto renderer = gWindow.get_renderer();
  const SDL_FRect playerBody = player.get_body();
  const f32 winW = f32(gWindow.get_width());
  const f32 winH = f32(gWindow.get_height());

  // Calculate the camera's view.
  const SDL_FPoint playerCenter(playerBody.x + (playerBody.w / 2.0f),
                                playerBody.y + (playerBody.h / 2.0f));
  const SDL_FRect view(playerCenter.x - (winW / 2.0f), playerCenter.y - (winH / 2.0f), winW, winH);

  // Draw tiles that are within the view.
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  for (const auto& tile : get_tiles()) {
    // Skip tiles that can't be seen.
    if (!do_rects_collide(view, tile)) {
      continue;
    }

    // Draw the tile relative to the view.
    const SDL_FRect dst(tile.x - view.x, tile.y - view.y, tile.w, tile.h);
    SDL_RenderFillRect(renderer, &dst);
  }

  // Draw coins that are within the view.
  SDL_FRect coinBody({}, {}, Coin::W, Coin::H);
  SDL_SetRenderDrawColor(renderer, 255, 225, 50, 255);
  for (const auto& coin : get_coins()) {
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
}
