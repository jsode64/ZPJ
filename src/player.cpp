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
      xSpeedMulti{1.0f}, xSpeed{BASE_X_SPEED * xSpeedMulti}, jumpSpeedMulti{0.0f}, jumpSpeed{BASE_JUMP_SPEED * jumpSpeedMulti},
      dashSpeedMulti{2.0f}, dashSpeed{xSpeed * dashSpeedMulti},
      batteryCapacity{1'000}, batteryCapacityIncrease{100}, batteryCost{BASE_BATTERY_COST}, batteryRemaining{1'000},
      dashCooldown{0}, numCoins{1000}, hasDoubleJump{false}, isDoubleJumpUnlocked{false},
      isDashUnlocked{false}, coyoteTime{0} {
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
    const Tile* oldGround = ground.value_or(nullptr);
    ground = std::nullopt;

    SDL_FPoint move = v;

    // Carry with platform you were already standing on.
    if (oldGround != nullptr) {
        move.x += oldGround->get_v().x;
        move.y += oldGround->get_v().y;
    }

    bool hitLeft = false;
    bool hitRight = false;
    bool hitTop = false;
    bool hitBottom = false;

    constexpr float EPS = 0.0001f;
    float remaining = 1.0f;

    for (int iter = 0; iter < 6 && remaining > EPS; iter++) {
        float bestT = 1.0f;
        const Tile* bestTile = nullptr;
        float bestNX = 0.0f;
        float bestNY = 0.0f;

        SDL_FRect bestTileStart{};
        SDL_FPoint bestTileMove{};

        for (const auto& tile : world.get_tiles()) {
            const SDL_FPoint tileMove = tile.get_v();
            const SDL_FRect tileEnd = tile.get_body();

            const SDL_FRect tileStart{
                tileEnd.x - tileMove.x,
                tileEnd.y - tileMove.y,
                tileEnd.w,
                tileEnd.h
            };

            const float relX = (move.x - tileMove.x) * remaining;
            const float relY = (move.y - tileMove.y) * remaining;

            if (std::abs(relX) < EPS && std::abs(relY) < EPS) {
                continue;
            }

            float xEntry, xExit;
            float yEntry, yExit;

            if (std::abs(relX) < EPS) {
                if (body.x + body.w <= tileStart.x || body.x >= tileStart.x + tileStart.w)
                    continue;

                xEntry = -INFINITY;
                xExit = INFINITY;
            } else if (relX > 0.0f) {
                xEntry = (tileStart.x - (body.x + body.w)) / relX;
                xExit = ((tileStart.x + tileStart.w) - body.x) / relX;
            } else {
                xEntry = ((tileStart.x + tileStart.w) - body.x) / relX;
                xExit = (tileStart.x - (body.x + body.w)) / relX;
            }

            if (std::abs(relY) < EPS) {
                if (body.y + body.h <= tileStart.y || body.y >= tileStart.y + tileStart.h) {
                    continue;
                }

                yEntry = -INFINITY;
                yExit = INFINITY;
            } else if (relY > 0.0f) {
                yEntry = (tileStart.y - (body.y + body.h)) / relY;
                yExit = ((tileStart.y + tileStart.h) - body.y) / relY;
            } else {
                yEntry = ((tileStart.y + tileStart.h) - body.y) / relY;
                yExit = (tileStart.y - (body.y + body.h)) / relY;
            }

            const float entryT = std::max(xEntry, yEntry);
            const float exitT = std::min(xExit, yExit);

            if (entryT > exitT || entryT < 0.0f || entryT > 1.0f)
                continue;

            if (entryT < bestT) {
                bestT = entryT;
                bestTile = &tile;
                bestTileStart = tileStart;
                bestTileMove = tileMove;

                if (xEntry > yEntry) {
                    bestNX = (relX > 0.0f) ? -1.0f : 1.0f;
                    bestNY = 0.0f;
                } else {
                    bestNX = 0.0f;
                    bestNY = (relY > 0.0f) ? -1.0f : 1.0f;
                }
            }
        }

        if (bestTile == nullptr) {
            body.x += move.x * remaining;
            body.y += move.y * remaining;
            break;
        }

        body.x += move.x * bestT * remaining;
        body.y += move.y * bestT * remaining;

        SDL_FRect tileAtHit{
            bestTileStart.x + bestTileMove.x * bestT * remaining,
            bestTileStart.y + bestTileMove.y * bestT * remaining,
            bestTileStart.w,
            bestTileStart.h
        };

        if (bestNX > 0.0f) hitLeft = true;
        if (bestNX < 0.0f) hitRight = true;
        if (bestNY > 0.0f) hitTop = true;
        if (bestNY < 0.0f) hitBottom = true;

        if ((hitLeft && hitRight) || (hitTop && hitBottom)) {
            kill();
            return;
        }

        if (bestNX != 0.0f) {
            v.x = 0.0f;
            move.x = bestTileMove.x;

            if (bestNX < 0.0f)
                body.x = tileAtHit.x - body.w;
            else
                body.x = tileAtHit.x + tileAtHit.w;
        }

        if (bestNY != 0.0f) {
            v.y = 0.0f;
            move.y = bestTileMove.y;

            if (bestNY < 0.0f) {
                body.y = tileAtHit.y - body.h;
                ground = bestTile;
                hasDoubleJump = false;
            } else {
                body.y = tileAtHit.y + tileAtHit.h;
            }
        }

        remaining *= (1.0f - bestT);
    }

    // Ground probe for standing still.
    if (!ground) {
        for (const auto& tile : world.get_tiles()) {
            const SDL_FRect t = tile.get_body();

            const bool overlapsX =
                body.x + body.w > t.x &&
                body.x < t.x + t.w;

            const bool touchingTop =
                std::abs((body.y + body.h) - t.y) < 0.1f;

            if (overlapsX && touchingTop && v.y >= 0.0f) {
                body.y = t.y - body.h;
                ground = &tile;
                hasDoubleJump = false;
                break;
            }
        }
    }
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
