#pragma once

#include <SDL3/SDL.h>

/** Do the rectangles intersect? */
constexpr bool do_rects_collide(const SDL_FRect a, const SDL_FRect b) {
    return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
}

/** Is the point inside the rectangle? */
constexpr bool is_point_in_rect(const SDL_FPoint p, const SDL_FRect r) {
    return r.x <= p.x && r.x + r.w >= p.x && r.y <= p.y && r.y + r.h >= p.y;
}
