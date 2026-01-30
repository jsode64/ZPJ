#include "window.h"

Window::Window(const char* title, i32 w, i32 h) : window(nullptr), renderer(nullptr), w(0), h(0), shouldClose(true) {
    if (SDL_CreateWindowAndRenderer(title, w, h, 0, &window, &renderer)) {
        shouldClose = false;
        update();

        // Settings.
        SDL_SetRenderVSync(renderer, 1);
    }
}

Window::~Window() {
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
