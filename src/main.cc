#include <exception>

#include "assets.h"
#include "audio.h"
#include "def.h"
#include "game.h"
#include "window.h"

i32 main() {
    // Load media stream.
    gAudio.init();
    gWindow.init();
    gAssets.load_assets();

    Game game;

    while (!gWindow.should_close()) {
        game.update();
        game.draw();

        gWindow.update();
    }
}
