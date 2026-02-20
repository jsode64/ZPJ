#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "def.h"

class Window {
private:
  /** The window's title. */
  static constexpr const char* TITLE = "ZPJ";

  /** The window's initial width. */
  static constexpr i32 INIT_W = 800;

  /** The window's initial height. */
  static constexpr i32 INIT_H = 450;

  /** The window's SDL window handle. */
  SDL_Window* window;

  /** The window's SDL renderer handle. */
  SDL_Renderer* renderer;

  /** The window's font. */
  TTF_Font* font;

  /** The window's width in pixels. */
  i32 w;

  /** The window's height in pixels. */
  i32 h;

  /** Is 'true` if the window should close (was closed, hit an error), `false`
   * if not. */
  bool shouldClose;

public:
  Window();

  ~Window();

  /** Returns the window's SDL renderer handle. */
  SDL_Renderer* get_renderer();

  /** Returns the window's width. */
  i32 get_width() const;

  /** Returns the window's height. */
  i32 get_height() const;

  /** Creates the given text rendered in a texture. */
  SDL_Texture* create_text(const std::string& text);

  /** Returns a texture made from the given file path. */
  SDL_Texture* create_texture(const char* path);

  /** Returns `true` if the window should close (was closed, hit an error),
   * `false` if not. */
  bool should_close() const;

  /** Updates the window state and presents the latest render. */
  void update();

  /** Initializes the window. */
  void init();
};

extern Window gWindow;
