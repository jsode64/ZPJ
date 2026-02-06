#include "window.h"

Window::Window(const char* title, const char* fontPath, i32 w, i32 h) : window(nullptr), renderer(nullptr), font(nullptr), w(0), h(0), shouldClose(true) {
    // Initialize audio
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        return;
    }

    // Initialize the font and render engines.
    if (!TTF_Init() || !SDL_CreateWindowAndRenderer(title, w, h, 0, &window, &renderer)) {
        return;
    }

    // Load font.
    font = TTF_OpenFont(fontPath, 16);

    shouldClose = false;
    update();

    // Settings.
    SDL_SetRenderVSync(renderer, 1);
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
