#include "def.h"
#include "window.h"

i32 main() {
    Window window("ZPJ", 800, 450);

    while (window.is_open()) {
        window.update();
    }
}
