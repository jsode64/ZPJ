#include "player.h"

#include <algorithm>
#include <exception>
#include "assets.h"
#include "coin.h"
#include "config.h"
#include "mixer.h"
#include "util.h"
#include "world.h"

Player::Player()
    : jumpKeyState(KEY_DOWN_SCANCODE(JUMP_KEY)), dashKeyState(KEY_DOWN_SCANCODE(DASH_KEY)), body{}, v{}, ground{},
      xSpeedMulti{1.0f}, xSpeed{BASE_X_SPEED * xSpeedMulti}, jumpSpeedMulti{3.0f}, jumpSpeed{BASE_JUMP_SPEED * jumpSpeedMulti},
      dashSpeedMulti{2.0f}, dashSpeed{xSpeed * dashSpeedMulti},
      batteryCapacity{1'000'000}, batteryCapacityIncrease{100}, batteryCost{BASE_BATTERY_COST}, batteryRemaining{1'000},
      dashCooldown{0}, numCoins{1000}, hasDoubleJump{false}, isDoubleJumpUnlocked{true},
      isDashUnlocked{false}, coyoteTime{0}, damagableCooldown{0} {
    init();
}

Player::~Player() {}

void Player::handle_input() {
    const auto keys = SDL_GetKeyboardState(nullptr);
    jumpKeyState.update();
    dashKeyState.update();

    // Horizontal movement.
    const bool isDashing = dashKeyState.was_just_pressed() && isDashUnlocked && (dashCooldown < 0);
    const f32 speed = isDashing ? dashSpeed : xSpeed;
    if (keys[LEFT_KEY]) {
        v.x = -speed;
        isFacingLeft = true;
    } else if (keys[RIGHT_KEY]) {
        v.x = speed;
        isFacingLeft = false;
    } else {
        v.x = 0.0f;
    }

    // Vertical movement.
    if (jumpKeyState.was_just_pressed() && (is_on_ground() || coyoteTime > 0 || (isDoubleJumpUnlocked && hasDoubleJump))) {
        v.y = -jumpSpeed;
        gMixer.play_sound(gAssets.jumpSound);
        hasDoubleJump = is_on_ground() || coyoteTime > 0;
        coyoteTime = 0;
    }
    v.y += GRAVITY;
}

void Player::handle_completion(const World& world) {
    if (world.are_fruits_collected() && do_rects_collide(body, World::DOOR)) {
        std::terminate();
    }
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
        bool hit = false;
        pre.y -= tileV.y;
        if (do_rects_collide(testBody, tileBody) && do_rects_collide(testBody, pre)) {
            hit = true;
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
                if (body.x + body.w > tileBody.x) {
                    body.x = tileBody.x - body.w;
                    hitRight = true;
                } else {
                    body.x = tileBody.x + tileBody.w;
                    hitLeft = true;
                }
            }
            v.x = 0.0f;
        }

        // Vertical collision check.
        testBody = SDL_FRect{x, y, body.w, body.h};
        if (do_rects_collide(testBody, tileBody)) {
            hit = true;
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

        // Check if touching a damageable tile.
        if (tile.is_damageable() && hit && damagableCooldown <= 0) {
            take_damage();
            tile.reset_damage_cooldown();
            damagableCooldown = 30;  // Player-side cooldown to prevent rapid damage
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

SDL_FPoint Player::get_texture_coordinates() const {
    const u32 nFrames = gWindow.get_frames();
    if (!is_on_ground()) {
        const f32 offset = (nFrames % 30 > 14) ? 8.0f : 0.0f;
        return {
            offset + ((v.y > 0.0f) ? 16.0f : 0.0f),
            8.0f,
        };
    } else if (v.x != 0.0f) {
        const u32 cycle = (nFrames / 12) % 4;
        const f32 x = (cycle == 1)
            ? 8.0f
            : (cycle == 3)
                ? 16.0f
                : 0.0f;
        return {
            x, 0.0f
        };
    } else {
        return {0.0f, 0.0f};
    }
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

void Player::take_damage() {
    batteryRemaining -= (batteryCapacity / 10);
}

bool Player::take_coins(i32 cost) {
    if (numCoins >= cost) {
        numCoins -= cost;
        return true;
    } else {
        return false;
    }
}

void Player::give_coins(const i32 _numCoins) { numCoins += _numCoins; }

void Player::increase_battery_capacity() { batteryCapacity += batteryCapacityIncrease; }

void Player::increase_battery_capacity_upgrade() {
    batteryCapacityIncrease *= 1.5f;
    batteryCapacityIncrease -= batteryCapacityIncrease % 10;  // Round to nearest 10 for cleaner numbers.
}

void Player::increase_battery_efficiency() { batteryCost -= 1; }

void Player::increase_speed() {
    xSpeedMulti += 0.25f; 
    xSpeed = BASE_X_SPEED * sqrt(xSpeedMulti);
    dashSpeed = xSpeed * dashSpeedMulti;
}

void Player::increase_jump() {
    jumpSpeedMulti += 1.0f;
    jumpSpeed = BASE_JUMP_SPEED * sqrt(jumpSpeedMulti);
}

void Player::increase_dash_speed() {
    dashSpeedMulti += 0.5f;
    dashSpeed = xSpeed * dashSpeedMulti;
}

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
    coyoteTime = 0;
}

void Player::update(World& world) {
    handle_input();
    handle_completion(world);
    handle_movement(world);

    batteryRemaining -= batteryCost;
    dashCooldown--;
    if (damagableCooldown > 0) {
        damagableCooldown--;
    }

    // Reset coyote time when on ground.
    if (is_on_ground()) {
        coyoteTime = MAX_COYOTE_TIME;
    } else if (coyoteTime > 0) {
        coyoteTime--;
    }
}

void Player::draw() const {
    const auto renderer = gWindow.get_renderer();
    const f32 winW = f32(gWindow.get_width());
    const f32 winH = f32(gWindow.get_height());

    // Draw the player body.
    const auto srcPos = get_texture_coordinates();
    const SDL_FRect src{
        srcPos.x, srcPos.y,
        8.0f, 8.0f,
    };
    const SDL_FRect bodyDst(
        (f32(gWindow.get_width()) - body.w) / 2.0f, (f32(gWindow.get_height()) - body.h) / 2.0f, body.w, body.h);
    SDL_RenderTextureRotated(renderer, gAssets.player.get(), &src, &bodyDst, 0.0, nullptr, 
isFacingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    // Draw the battery meter.
    const f32 percentBattery = f32(batteryRemaining) / f32(batteryCapacity);
    const f32 danger = 1.0f - percentBattery;
    const f32 t = f32(gWindow.get_frames()) / 60.0f;
    const f32 flashHz = 1.0f + (2.0f * danger * danger);
    const f32 pulse = 0.5f + (0.5f * std::sin(6.2831853f * flashHz * t));
    const f32 redAmount = std::clamp(danger * (0.35f + (0.65f * pulse)), 0.0f, 1.0f);
    const SDL_FRect meterLeftDst(0.0f, 0.0f, winW * percentBattery, 20.0f);
    const SDL_FRect meterRightDst(meterLeftDst.w, 0.0f, winW * (1.0f - percentBattery), 20.0f);
    SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f - redAmount, 1.0f - redAmount, 1.0f);
    SDL_RenderFillRect(renderer, &meterLeftDst);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &meterRightDst);
}
