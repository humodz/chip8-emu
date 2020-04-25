#include <SDL2/SDL.h>
#include "util.h"

namespace gfx {
  class Renderer {
  public:
    SDL_Renderer *data;

    Renderer(SDL_Window *window, int index, Uint32 flags) {
      data = SDL_CreateRenderer(window, index, flags);
      if (data == nullptr) {
        GFX_SDL_ERROR();
      }
    }

    ~Renderer() {
      if (data != nullptr) {
        SDL_DestroyRenderer(data);
      }
    }

    Renderer(Renderer&) = delete;
  };
}