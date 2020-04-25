#ifndef SURFACE_H
#define SURFACE_H

#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "util.h"

namespace gfx {
  class Surface {
  public:
    SDL_Surface *data;

    friend void swap(Surface &lhs, Surface &rhs);

    Surface(SDL_Surface *surface = nullptr):
      data(surface) {}

    ~Surface() {
      if (data != nullptr) {
        SDL_FreeSurface(data);
      }
    }

    Surface(const Surface &that) = delete;

    Surface &operator=(Surface &&that) {
      swap(*this, that);
      return *this;
    }

    Surface(Surface &&that) {
      swap(*this, that);
    }

    static Surface fromFile(const char *filename, const SDL_PixelFormat *optimizeTo = nullptr) {
      SDL_Surface *sdlSurface = IMG_Load(filename);

      if (sdlSurface == nullptr) {
        GFX_IMG_ERROR();
      }

      SDL_Surface *origSurface = sdlSurface;

      if (optimizeTo != nullptr) {
        sdlSurface = SDL_ConvertSurface(sdlSurface, optimizeTo, 0);
        SDL_FreeSurface(origSurface);

        if (sdlSurface == nullptr) {
          GFX_SDL_ERROR();
        }
      }

      return {sdlSurface};
    }
  };

  void swap(Surface &lhs, Surface &rhs) {
    using std::swap;
    swap(lhs.data, rhs.data);
  }
}

#endif