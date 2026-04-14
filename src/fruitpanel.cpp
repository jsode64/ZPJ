#include "fruitpanel.h"
#include "window.h"

void FruitPanel::init() {
    panelTexture.load_bmp("assets/panel.bmp");

    // Load the first frame of each fruit as the slot icon.
    // Order: orange, pineapple, apple, blueberry, strawberry
    iconTextures[0].load_bmp("assets/orangeOne.bmp");
    iconTextures[1].load_bmp("assets/pineappleOne.bmp");
    iconTextures[2].load_bmp("assets/appleOne.bmp");
    iconTextures[3].load_bmp("assets/blueberryOne.bmp");
    iconTextures[4].load_bmp("assets/strawberryOne.bmp");
}

void FruitPanel::collect(int slot) {
    if (slot >= 0 && slot < 5) {
        collected[slot] = true;
    }
}

void FruitPanel::draw() const {
    auto renderer = gWindow.get_renderer();

    // Draw the panel background.
    const SDL_FRect panelDst = {PANEL_X, PANEL_Y, PANEL_W, PANEL_H};
    SDL_RenderTexture(renderer, panelTexture.get(), nullptr, &panelDst);

    // Draw collected fruit icons over their slots.
    for (int i = 0; i < 5; i++) {
        if (!collected[i])
            continue;

        const float iconX = PANEL_X + SLOT_CENTERS[i] - (ICON_SIZE / 2.0f);
        const float iconY = PANEL_Y + SLOT_CENTER_Y - (ICON_SIZE / 2.0f);
        const SDL_FRect iconDst = {iconX, iconY, ICON_SIZE, ICON_SIZE};
        SDL_RenderTexture(renderer, iconTextures[i].get(), nullptr, &iconDst);
    }
}