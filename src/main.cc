#include "def.h"
#include "game.h"
#include "window.h"

i32 main() {
    Window window("ZPJ", "assets/comic_sans.ttf", 800, 450);
    Game game(window);

    while (!window.should_close()) {
        game.update();
        game.draw(window);

        window.update();
    }
}
