#include "player.h"

#include "config.h"
#include "util.h"

Player::Player() : body{}, v{},
    batteryCapacity(STARTING_BATTERY_CAPACITY), battery(-1), 
    onGround(false) {
        init();
}

Player::~Player() {

}

void Player::handle_input() {
    const auto keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_A]) {
        v.x = -X_SPEED;
    } else if (keys[SDL_SCANCODE_D]) {
        v.x = X_SPEED;
    } else {
        v.x = 0.0f;
    }

    if (onGround && keys[SDL_SCANCODE_SPACE]) {
        v.y = -JUMP_SPEED;
    } else {
        v.y += GRAVITY;
    }
}

void Player::handle_movement(const World& world) {
    onGround = false;

    // Check for horizontal collisions.
    body.x += v.x;
    for (const auto& tile: world.get_tiles()) {
        if (do_rects_collide(tile, body)) {
            if (v.x >= 0.0f) { // Colliding from the left.
                body.x = tile.x - body.w;
            } else { // Colliding from the right.
                body.x = tile.x + tile.w;
            }
            v.x = 0;
        }
    }

    // Check for vertical collisions.
    body.y += v.y;
    for (const auto& tile: world.get_tiles()) {
        if (do_rects_collide(tile, body)) {
            if (v.y >= 0.0f) { // Colliding from the top.
                body.y = tile.y - body.h;
                onGround = true;
            } else { // Colliding from the bottom.
                body.y = tile.y + tile.h;
            }
            v.y = 0;
        }
    }
}

void Player::handle_collecting(World& world) {
    // Check for collected coins.
    SDL_FRect coinBody(
        {},
        {},
        Coin::W,
        Coin::H
    );
    for (auto& coin: world.get_coins_mut()) {
        coinBody.x = coin.get_x();
        coinBody.y = coin.get_y();

        // Collect the coin if it's active and touching.
        if (coin.is_active() && do_rects_collide(body, coinBody)) {
            coin.collect();
            batteryCapacity += 30;
        }
    }
}

SDL_FRect Player::get_body() const {
    return body;
}

bool Player::is_out_of_battery() const {
    return battery < 0;
}

void Player::init() {
    body = {
        0.0f,
        0.0f,
        W,
        H,
    };
    v = { 0.0f, 0.0f };
    battery = batteryCapacity;
    onGround = false;
}

void Player::update(World& world) {
    handle_input();
    handle_movement(world);
    handle_collecting(world);

    battery--;
}

void Player::draw(Window& window) const {
    auto renderer = window.get_renderer();
    const SDL_FRect dst(
        (f32(window.get_width()) - body.w) / 2.0f,
        (f32(window.get_height()) - body.h) / 2.0f,
        body.w,
        body.h
    );

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &dst);
}
