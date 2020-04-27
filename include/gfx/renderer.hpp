#ifndef GFX_RENDERER_HPP
#define GFX_RENDERER_HPP

#include <SDL2/SDL.h>

namespace gfx {
  class Renderer {
  public:
    friend void swap(Renderer &lhs, Renderer &rhs);

    SDL_Renderer *data;

    Renderer(SDL_Renderer *data = nullptr);
    Renderer(SDL_Window *window, int index, Uint32 flags);
    ~Renderer();
    Renderer(Renderer&) = delete;
    Renderer(Renderer &&that);
    Renderer &operator=(Renderer &&that);
  };

  void swap(Renderer &lhs, Renderer &rhs);
}

#endif