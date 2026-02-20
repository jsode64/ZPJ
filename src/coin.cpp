#include "coin.h"

Coin::Coin(f32 x, f32 y) : x(x), y(y), isActive(true) {}

f32 Coin::get_x() const { return x; }

f32 Coin::get_y() const { return y; }

bool Coin::is_active() const { return isActive; }

void Coin::collect() { isActive = false; }
