#pragma once

#include <SDL3/SDL.h>

/** @brief Returns `true` if the rectangle collide, `false` if not. */
constexpr bool do_rects_collide(const SDL_FRect a, const SDL_FRect b) {
    return a.x < b.x + b.w
        && a.x + a.w > b.x
        && a.y < b.y + b.h
        && a.y + a.h > b.y;
}
