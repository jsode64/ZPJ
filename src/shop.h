#pragma once

#include <SDL3/SDL.h>

#include "menu.h"
#include "def.h"

class Shop: public Menu {
private:
    enum class ButtonID {
        RestoreBattery = 0,
        Exit = 1,
    };

    int player_coins;
    int player_battery;
    int player_shield;
    int last_clicked_button;

    static constexpr i32 BUTTON_WIDTH = 150;
    static constexpr i32 BUTTON_HEIGHT = 50;
    static constexpr i32 BUTTON_SPACING = 20;

public:
    Shop();

    /** @brief Update shop state (handle mouse input). */
    void update(i32 mouse_x, i32 mouse_y, bool mouse_clicked) override;

    /** @brief Draw the shop menu. */
    void draw(SDL_Renderer* renderer) const override;

    /** @brief Set player stats for the shop. */
    void set_player_stats(int coins, int battery);

    /** @brief Get the last button that was sent (returns -1 if none, then int from ButtonID). */
    int get_last_action();
};