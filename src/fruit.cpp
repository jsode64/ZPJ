#include "fruit.h"
#include <cmath>

Fruit::Fruit(f32 x, f32 y, FruitType type)
    : x(x), y(y), type(type), isActive(true), frame(0), animTick(0), floatTick(0.0f) {}

f32 Fruit::get_x() const { return x; }

f32 Fruit::get_y() const { return y; }

SDL_FRect Fruit::get_body() const { return {x, y, W, H}; }

FruitType Fruit::get_type() const { return type; }

int Fruit::get_slot() const { return static_cast<int>(type); }

bool Fruit::is_active() const { return isActive; }

void Fruit::collect() { isActive = false; }

void Fruit::update() const {
    animTick++;
    if (animTick >= TICKS_PER_FRAME) {
        animTick = 0;
        frame = (frame + 1) % 4;
    }
    floatTick += 0.02f;
}

int Fruit::get_frame() const { return frame; }

f32 Fruit::get_float_offset() const { return std::sin(floatTick) * FLOAT_AMPLITUDE; }