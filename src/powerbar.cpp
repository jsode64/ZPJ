#include "powerbar.h"
#include "window.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

void PowerBar::init() {
    barTexture.load_bmp("assets/powerBar.bmp");
    juiceTexture.load_bmp("assets/juice.bmp");
}

static float rand_float(float min, float max) { return min + (float(std::rand()) / float(RAND_MAX)) * (max - min); }

void PowerBar::draw(int batteryRemaining, int batteryCapacity) const {
    auto renderer = gWindow.get_renderer();
    const float winW = float(gWindow.get_width());
    const float winH = float(gWindow.get_height());

    const float barX = winW - BAR_W - MARGIN_RIGHT;
    const float barY = (winH - BAR_H) / 2.0f;

    // Draw the empty bar background.
    const SDL_FRect barDst = {barX, barY, BAR_W, BAR_H};
    SDL_RenderTexture(renderer, barTexture.get(), nullptr, &barDst);

    // Calculate fill percentage (clamped 0-1).
    const float pct = std::clamp(float(batteryRemaining) / float(batteryCapacity), 0.0f, 1.0f);

    if (pct <= 0.0f)
        return;

    // Get juice texture dimensions.
    float texW, texH;
    SDL_GetTextureSize(juiceTexture.get(), &texW, &texH);

    // Crop from the top - src rect starts lower as battery drains.
    const float srcY = texH * (1.0f - pct);
    const SDL_FRect juiceSrc = {0.0f, srcY, texW, texH * pct};

    // Dst: center juice horizontally in bar, anchored to bottom of bar.
    const float juiceX = barX + (BAR_W - JUICE_W) / 2.0f;
    const float juiceH = BAR_H * pct * 0.6f;
    const float juiceY = barY + (BAR_H - juiceH) - 100.0f;
    const SDL_FRect juiceDst = {juiceX, juiceY, JUICE_W, juiceH};

    // Clip to bar bounds so juice never escapes.
    const SDL_Rect clipRect = {int(barX), int(barY), int(BAR_W), int(BAR_H)};
    SDL_SetRenderClipRect(renderer, &clipRect);

    // Tint red when below 20%.
    if (pct <= 0.2f) {
        SDL_SetTextureColorMod(juiceTexture.get(), 255, 0, 0);
    } else {
        SDL_SetTextureColorMod(juiceTexture.get(), 255, 255, 255);
    }

    SDL_RenderTexture(renderer, juiceTexture.get(), &juiceSrc, &juiceDst);

    // Initialize particles at the top surface of the juice.
    if (!particlesInit) {
        for (auto& p : particles) {
            p.x = rand_float(2.0f, JUICE_W - 2.0f);
            p.y = rand_float(-juiceH, 0.0f); // stagger initial positions above juice top
            p.speedY = rand_float(0.4f, 1.4f);
            p.speedX = rand_float(-0.4f, 0.4f);
            p.size = rand_float(1.5f, 3.5f);
        }
        particlesInit = true;
    }

    // Draw light green bubbles floating up from juice surface.
    SDL_SetRenderDrawColor(renderer, 180, 255, 180, 200);
    for (auto& p : particles) {
        // Move bubble upward (negative Y = up in SDL).
        p.y -= p.speedY;
        p.x += p.speedX;

        // Reset when bubble floats too far up or drifts sideways.
        if (p.y < -juiceH || p.x < 0.0f || p.x > JUICE_W) {
            p.x = rand_float(2.0f, JUICE_W - 2.0f);
            p.y = 0.0f; // respawn at top surface of juice
            p.speedY = rand_float(0.4f, 1.4f);
            p.speedX = rand_float(-0.4f, 0.4f);
            p.size = rand_float(1.5f, 3.5f);
        }

        // Draw bubble relative to juiceY (top of juice).
        const SDL_FRect bubble = {juiceX + p.x - p.size, juiceY + p.y - p.size, p.size * 2.0f, p.size * 2.0f};
        SDL_RenderFillRect(renderer, &bubble);
    }

    // Clear clip rect.
    SDL_SetRenderClipRect(renderer, nullptr);
}