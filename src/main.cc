#include "assets.h"
#include "def.h"
#include "game.h"
#include "mixer.h"
#include "window.h"

i32 main() {
  // Load media stream.
  gWindow.init();
  gMixer.init();
  gAssets.load_assets();

  Game game;

  while (!gWindow.should_close()) {
    game.update();
    game.draw();

    gWindow.update();
  }

  return 0;
}
