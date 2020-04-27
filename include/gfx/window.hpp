#ifndef GFX_WINDOW_HPP
#define GFX_WINDOW_HPP

#include <SDL2/SDL.h>

namespace gfx {
  class Window {
  public:
    friend void swap(Window &lhs, Window &rhs);

    SDL_Window *data;

    Window(SDL_Window *data = nullptr);
    Window(const char* title, int x, int y, int w, int h, Uint32 flags);
    ~Window();
    Window(Window&) = delete;
    Window(Window &&that);
    Window &operator=(Window &&that);

    SDL_Surface *surface() const;
    SDL_PixelFormat *pixelFormat() const;
  };

  void swap(Window &lhs, Window &rhs);
}

#endif