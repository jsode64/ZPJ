#include "window.h"

Window::Window(const char* title, i32 w, i32 h) : window(nullptr), renderer(nullptr), running(false) {
    running = SDL_CreateWindowAndRenderer(title, w, h, 0, &window, &renderer);
}

Window::~Window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

bool Window::is_open() const {
    return running;
}

void Window::update() {
    // Handle events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }

    // Show render and clear
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}
