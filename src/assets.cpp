#include "assets.h"

Assets gAssets;

Assets::Assets() : coinCollectSound{} {

}

bool Assets::load_assets() {
    coinCollectSound.load_wav("assets/coin.wav");
}
