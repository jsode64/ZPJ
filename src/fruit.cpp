#include "fruit.h"

Fruit::Fruit(f32 x, f32 y, bool isActive) : x(x), y(y), isActive(isActive) {}

f32 Fruit::get_x() const { return x; }

f32 Fruit::get_y() const { return y; }

bool Fruit::is_active() const { return isActive; }

void Fruit::collect() { isActive = false; }
