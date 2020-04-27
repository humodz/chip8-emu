#ifndef GFX_SURFACE_HPP
#define GFX_SURFACE_HPP

#include <SDL2/SDL.h>

namespace gfx {
  class Surface {
  public:
    SDL_Surface *data;

    friend void swap(Surface &lhs, Surface &rhs);

    Surface(SDL_Surface *surface = nullptr);
    ~Surface();
    Surface(const Surface &that) = delete;
    Surface(Surface &&that);
    Surface &operator=(Surface &&that);

    static Surface fromFile(
      const char *filename, const SDL_PixelFormat *optimizeTo = nullptr
    );
  };

  void swap(Surface &lhs, Surface &rhs);
}

#endif