#include <SDL2/SDL.h>
#include "util.h"

namespace gfx {
  class Window {
  public:
    SDL_Window *data;

    Window(const char* title, int x, int y, int w, int h, Uint32 flags) {
      data = SDL_CreateWindow(title, x, y, w, h, flags);
      if (data == nullptr) {
        GFX_SDL_ERROR();
      }
    }

    ~Window() {
      if (data != nullptr) {
        SDL_DestroyWindow(data);
      }
    }

    Window(Window&) = delete;

    SDL_Surface *surface() const {
      auto *surface = SDL_GetWindowSurface(data);
      if (surface == nullptr) {
        GFX_SDL_ERROR();
      }
      return surface;
    }

    SDL_PixelFormat *pixelFormat() const {
      auto formatEnum = SDL_GetWindowPixelFormat(data);

      if (formatEnum == SDL_PIXELFORMAT_UNKNOWN) {
        GFX_SDL_ERROR();
      }

      auto *format = SDL_AllocFormat(SDL_GetWindowPixelFormat(data));

      if (format == nullptr) {
        GFX_SDL_ERROR();
      }

      return format;
    }
  };
}