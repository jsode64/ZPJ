#include "player.h"
#include "config.h"
#include "util.h"
#include "audio_mixer.h"

Player::Player() : body{}, v{},
    numCoins(0),
    batteryCapacity(STARTING_BATTERY_CAPACITY), batteryRemaining(-1), 
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
        g_audio_mixer.play_jump_sound(); // play jump sound
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
            g_audio_mixer.play_coin_sound(); //play coin sound
        }
    }
}

SDL_FRect Player::get_body() const {
    return body;
}

bool Player::is_out_of_battery() const {
    return batteryRemaining < 0;
}

void Player::init() {
    body = {
        0.0f,
        0.0f,
        W,
        H,
    };
    v = { 0.0f, 0.0f };
    batteryRemaining = batteryCapacity;
    onGround = false;
}

void Player::update(World& world) {
    handle_input();
    handle_movement(world);
    handle_collecting(world);

    batteryRemaining--;
}

void Player::draw(Window& window) const {
    const auto renderer = window.get_renderer();
    const f32 winW = f32(window.get_width());
    const f32 winH = f32(window.get_height());

    // Draw the player body.
    const SDL_FRect bodyDst(
        (f32(window.get_width()) - body.w) / 2.0f,
        (f32(window.get_height()) - body.h) / 2.0f,
        body.w,
        body.h
    );
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bodyDst);

    // Draw the battery meter.
    const f32 percentBattery = f32(batteryRemaining) / f32(batteryCapacity);
    const SDL_FRect meterLeftDst(
        0.0f,
        0.0f,
        winW * percentBattery,
        20.0f
    );
    const SDL_FRect meterRightDst(
        meterLeftDst.w,
        0.0f,
        winW * (1.0f - percentBattery),
        20.0f
    );
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &meterLeftDst);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &meterRightDst);
}
