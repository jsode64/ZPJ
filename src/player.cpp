#include "player.h"

#include "config.h"
#include "util.h"

Player::Player() : body(0.0f, 0.0f, W, H), v(0.0f, 0.0f), onGround(false) {

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

    // Check for horizontal collision.
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

    // Check for vertical collision.
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

SDL_FRect Player::get_body() const {
    return body;
}

void Player::update(const World& world) {
    handle_input();
    handle_movement(world);
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
