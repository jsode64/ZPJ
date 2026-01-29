#include "def.h"
#include "player.h"
#include "window.h"
#include "world.h"

i32 main() {
    Window window("ZPJ", 800, 450);
    World world;
    Player player;

    while (!window.should_close()) {
        // Update game state.
        player.update(world);

        // Draw game.
        world.draw(window, player);
        player.draw(window);

        window.update();
    }
}
