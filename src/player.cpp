#include "player.h"

#include <algorithm>
#include <print>

#include "assets.h"
#include "coin.h"
#include "config.h"
#include "mixer.h"
#include "util.h"
#include "world.h"

Player::Player()
    : jumpKeyState(KEY_DOWN_SCANCODE(JUMP_KEY)), dashKeyState(KEY_DOWN_SCANCODE(DASH_KEY)), body{}, v{}, ground{},
      xSpeed{BASE_X_SPEED}, jumpSpeed{BASE_JUMP_SPEED}, batteryCapacity{1'000'000}, batteryCost{BASE_BATTERY_COST},
      batteryRemaining{1'000'000}, dashCooldown{0}, numCoins{0}, hasDoubleJump{false}, isDoubleJumpUnlocked{false},
      isDashUnlocked{false} {
    init();
}

Player::~Player() {}

void Player::handle_input() {
    const auto keys = SDL_GetKeyboardState(nullptr);
    jumpKeyState.update();
    dashKeyState.update();

    // Horizontal movement.
    const bool isDashing = dashKeyState.was_just_pressed() && isDashUnlocked && (dashCooldown < 0);
    const f32 speed = isDashing ? DASH_SPEED : xSpeed;
    if (keys[LEFT_KEY]) {
        v.x = -speed;
    } else if (keys[RIGHT_KEY]) {
        v.x = speed;
    } else {
        v.x = 0.0f;
    }

    // Vertical movement.
    if (jumpKeyState.was_just_pressed() && (is_on_ground() || (isDoubleJumpUnlocked && hasDoubleJump))) {
        v.y = -jumpSpeed;
        gMixer.play_sound(gAssets.jumpSound);
        hasDoubleJump = is_on_ground();
    }
    v.y += GRAVITY;
}

void Player::handle_movement(const World& world) {
    float x = body.x + v.x;
    float y = body.y + v.y;

    /* Tracking squishing. Each is tracking if hit from that direction, for
    example, `hitDown` will be true when hit from below (landing/standing) */
    bool hitUp = false;
    bool hitDown = false;
    bool hitLeft = false;
    bool hitRight = false;

    // If on a tile, move with it.
    if (ground.has_value()) {
        x += ground.value()->get_v().x;
    }

    ground = std::nullopt;

    for (const auto& tile : world.get_tiles()) {
        // Horizontal collision check.
        const auto tileBody = tile.get_body();
        const auto tileV = tile.get_v();
        auto testBody = SDL_FRect{x, body.y, body.w, body.h};
        auto pre = tileBody;
        pre.y -= tileV.y;
        if (do_rects_collide(testBody, tileBody) && do_rects_collide(testBody, pre)) {
            if (v.x > 0.0f) {
                if (tileV.x > v.x) {
                    // Both moving right, tile faster:
                    x = tileBody.x + tileBody.w;
                    v.x = tileV.x;
                    hitLeft = true;
                } else {
                    x = tileBody.x - body.w;
                    v.x = 0.0f;
                    hitRight = true;
                }
            } else if (v.x < 0.0f) {
                if (tileV.x < v.x) {
                    // Both moving left, tile faster:
                    x = tileBody.x - body.w;
                    v.x = tileV.x;
                    hitRight = true;
                } else {
                    x = tileBody.x + tileBody.w;
                    v.x = 0.0f;
                    hitLeft = true;
                }
            } else {
                // Player not moving horizontally, resolve based on previous position.
                if (body.x + body.w <= tileBody.x) {
                    // We were completely to the left of the tile.
                    x = tileBody.x - body.w;
                    hitRight = true;
                } else if (body.x >= tileBody.x + tileBody.w) {
                    // We were completely to the right of the tile.
                    x = tileBody.x + tileBody.w;
                    hitLeft = true;
                } else {
                    // We were overlapping or tile hit us - use center positions.
                    if (body.x + body.w / 2.0f < tileBody.x + tileBody.w / 2.0f) {
                        x = tileBody.x - body.w;
                        hitRight = true;
                    } else {
                        x = tileBody.x + tileBody.w;
                        hitLeft = true;
                    }
                }
                v.x = 0.0f;
            }
        }

        // Vertical collision check.
        testBody = SDL_FRect{x, y, body.w, body.h};
        if (do_rects_collide(testBody, tileBody)) {
            if (v.y >= 0.0f) {
                y = tileBody.y - body.h;

                // On ground.
                hasDoubleJump = isDoubleJumpUnlocked;
                ground = &tile;
                hitDown = true;
            } else {
                y = tileBody.y + tileBody.h + tileV.y;
                hitUp = true;
            }
            v.y = std::max(0.f, tileV.y);
        }

        // Check for squish.
        if ((hitLeft && hitRight) || (hitUp && hitDown)) {
            kill();
            return;
        }
    }

    // Move to new position.
    body.x = x;
    body.y = y;
}

SDL_FRect Player::get_body() const { return body; }

i32 Player::get_coins() const { return numCoins; }

i32 Player::get_battery_capacity() const { return batteryCapacity; }

bool Player::has_dash_unlocked() const { return isDashUnlocked; }

bool Player::has_double_jump_unlocked() const { return isDoubleJumpUnlocked; }

bool Player::is_on_ground() const { return ground.has_value(); }

bool Player::is_alive() const { return batteryRemaining >= 0; }

void Player::kill() { batteryRemaining = -1; }

bool Player::is_out_of_battery() const { return batteryRemaining < 0; }

bool Player::take_coins(i32 cost) {
    if (numCoins >= cost) {
        numCoins -= cost;
        return true;
    } else {
        return false;
    }
}

void Player::give_coins(const i32 _numCoins) { numCoins += _numCoins; }

void Player::increase_battery_capacity() { batteryCapacity += 100; }

void Player::increase_battery_efficiency() { batteryCost -= 2; }

void Player::increase_speed() { xSpeed += 0.5f; }

void Player::increase_jump() { jumpSpeed += 1.0f; }

void Player::unlock_dash() { isDashUnlocked = true; }

void Player::unlock_double_jump() { isDoubleJumpUnlocked = true; }

void Player::init() {
    body = {
        -(W / 2.0f),
        -(H / 2.0f),
        W,
        H,
    };
    v = {0.0f, 0.0f};
    batteryRemaining = batteryCapacity;
    ground = std::nullopt;
}

void Player::update(World& world) {
    handle_input();
    handle_movement(world);

    batteryRemaining -= batteryCost;
    dashCooldown--;
}

void Player::draw() const {
    const auto renderer = gWindow.get_renderer();
    const f32 winW = f32(gWindow.get_width());
    const f32 winH = f32(gWindow.get_height());

    // Draw the player body.
    const SDL_FRect bodyDst(
        (f32(gWindow.get_width()) - body.w) / 2.0f, (f32(gWindow.get_height()) - body.h) / 2.0f, body.w, body.h);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bodyDst);

    // Draw the battery meter.
    const f32 percentBattery = f32(batteryRemaining) / f32(batteryCapacity);
    const SDL_FRect meterLeftDst(0.0f, 0.0f, winW * percentBattery, 20.0f);
    const SDL_FRect meterRightDst(meterLeftDst.w, 0.0f, winW * (1.0f - percentBattery), 20.0f);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &meterLeftDst);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &meterRightDst);
}
