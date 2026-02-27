#include "key.h"

Key::Key(KeyDownFn keyDownFn) : keyDownFn(keyDownFn), isDown(false), wasJustPressed(false) {}

bool Key::is_down() const { return isDown; }

bool Key::was_just_pressed() const { return wasJustPressed; }

void Key::update() {
    const bool isDownNow = keyDownFn();

    wasJustPressed = isDownNow && !isDown;
    isDown = isDownNow;
}
