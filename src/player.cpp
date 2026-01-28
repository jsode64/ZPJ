#include "player.h"

Player::Player() : body(0.0f, 0.0f, W, H), v(0.0f, 0.0f) {

}

Player::~Player() {

}

SDL_FRect Player::get_body() const {
    return body;
}

void Player::update() {
    const auto keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_LEFT]) {
        v.x = -X_SPEED;
    } else if (keys[SDL_SCANCODE_RIGHT]) {
        v.x = X_SPEED;
    } else {
        v.x = 0.0f;
    }

    if (keys[SDL_SCANCODE_UP]) {
        v.y = Y_SPEED;
    } else {
        if (v.y > -Y_SPEED) {
            v.y -= GRAVITY;
            if (v.y < -Y_SPEED) {
                v.y = -Y_SPEED;
            }
        } 
    }

    body.x += v.x;
    body.y += v.y;


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
