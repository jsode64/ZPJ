#include "def.h"
#include "game.h"
#include "window.h"

i32 main() {
    Window window("ZPJ", 800, 450);
    Game game;

    while (!window.should_close()) {
        game.update();
        game.draw(window);

        window.update();
    }
}
