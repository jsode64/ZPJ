#include "player.h"

#include <algorithm>

#include "assets.h"
#include "config.h"
#include "util.h"
#include "mixer.h"

Player::Player() : jumpKeyState(JUMP_KEY), dashKeyState(DASH_KEY),
    body{}, v{},
    xSpeed{BASE_X_SPEED}, jumpSpeed{BASE_JUMP_SPEED},
    batteryCapacity(STARTING_BATTERY_CAPACITY), batteryCost{BASE_BATTERY_COST}, batteryRemaining(-1),
    dashCooldown(0),
    numCoins(0),
    onGround(false), hasDoubleJump(false),
    isDoubleJumpUnlocked(true), isDashUnlocked(true) {
        init();
}

Player::~Player() {

}

void Player::handle_input() {
    const auto keys = SDL_GetKeyboardState(nullptr);
    jumpKeyState.update();
    dashKeyState.update();

    // Horizontal movement.
    const bool isDashing = dashKeyState.was_just_pressed() && isDashUnlocked && (dashCooldown < 0);
    const f32 xSpeed = isDashing ? DASH_SPEED : xSpeed;
    if (keys[LEFT_KEY]) {
        v.x = std::min(v.x, -xSpeed);
    } else if (keys[RIGHT_KEY]) {
        v.x = std::max(v.x, xSpeed);
    } else {
        v.x = 0.0f;
    }

    // Vertical movement.
    if (jumpKeyState.was_just_pressed() && (onGround || hasDoubleJump)) {
        v.y = -jumpSpeed;
        gMixer.play_sound(gAssets.jumpSound);
        hasDoubleJump = onGround;
    }
    v.y += GRAVITY;
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
                hasDoubleJump = isDoubleJumpUnlocked;
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
            gMixer.play_sound(gAssets.coinCollectSound);
        }
    }
}

SDL_FRect Player::get_body() const {
    return body;
}

bool Player::is_out_of_battery() const {
    return batteryRemaining < 0;
}

i32 Player::get_coins() const {
    return static_cast<i32>(numCoins);
}

i32 Player::get_battery() const {
    return static_cast<i32>(batteryCapacity);
}

void Player::increase_battery_capacity() {
    batteryCapacity += 30;
}

void Player::increase_battery_efficiency() {
    batteryCost -= 2;
}

void Player::increase_speed() {
    xSpeed += 0.5f;
}

void Player::increase_jump() {
    jumpSpeed += 1.0f;
}

void Player::take_coins(i32 cost) {
    numCoins = std::max(numCoins - cost, 0);
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

    batteryRemaining -= batteryCost;
    dashCooldown--;
}

void Player::draw() const {
    const auto renderer = gWindow.get_renderer();
    const f32 winW = f32(gWindow.get_width());
    const f32 winH = f32(gWindow.get_height());

    // Draw the player body.
    const SDL_FRect bodyDst(
        (f32(gWindow.get_width()) - body.w) / 2.0f,
        (f32(gWindow.get_height()) - body.h) / 2.0f,
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
