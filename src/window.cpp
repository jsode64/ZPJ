#include "window.h"

#include <exception>
#include <format>

Window gWindow;

Window::Window() : window{nullptr}, renderer{nullptr}, font{nullptr}, w{0}, h{0}, shouldClose{true} {

}

Window::~Window() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

SDL_Renderer* Window::get_renderer() {
    return renderer;
}

i32 Window::get_width() const {
    return w;
}

i32 Window::get_height() const {
    return h;
}

SDL_Texture* Window::create_text(const std::string& text) {
    SDL_Surface* surface = TTF_RenderText_Blended(
        font,
        text.data(),
        text.size(),
        SDL_Color(255, 255, 255, 255)
    );
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);

    return texture;
}

SDL_Texture* Window::create_texture(const char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);

    return texture;
}

bool Window::should_close() const {
    return shouldClose;
}

void Window::update() {
    // Handle closing.
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            shouldClose = true;
        }
    }

    // Show render and clear.
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Update info.
    SDL_GetWindowSizeInPixels(window, &w, &h);
}

void Window::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        const auto e = std::format("Failed to intiialize SDL video: \"{}\"", SDL_GetError());
        throw std::runtime_error(e);
    }

    // Initialize the font and render engines.
    if (!SDL_CreateWindowAndRenderer(TITLE, INIT_W, INIT_H, 0, &window, &renderer)) {
        const auto e = std::format("Failed to create SDL window/renderer: \"{}\"", SDL_GetError());
        throw std::runtime_error(e);
    }

    shouldClose = false;
    update();

    // Settings.
    SDL_SetRenderVSync(renderer, 1);
}
