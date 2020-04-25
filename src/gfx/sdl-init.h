#ifndef MY_SDL_INIT_H
#define MY_SDL_INIT_H

#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "util.h"

namespace gfx {
  class Sdl2Init {
  public:
    int status;

    Sdl2Init(Uint32 flags) {
      status = SDL_Init(flags);
      if (status < 0) {
        GFX_SDL_ERROR();
      }
    }

    ~Sdl2Init() {
      if (status >= 0) {
        SDL_Quit();
      }
    }

    Sdl2Init(Sdl2Init&) = delete;
  };

  class Sdl2ImgInit {
  public:
    int status;

    Sdl2ImgInit(int flags) {
      status = IMG_Init(flags) & flags;
      if (status != 0) {
        GFX_IMG_ERROR();
      }
    }

    ~Sdl2ImgInit() {
      if (status == 0) {
        IMG_Quit();
      }
    }

    Sdl2ImgInit(Sdl2ImgInit&) = delete;
  };

  class Sdl2TtfInit {
  public:
    int status;

    Sdl2TtfInit() {
      status = TTF_Init();
      if (status == -1) {
        GFX_TTF_ERROR();
      }
    }

    ~Sdl2TtfInit() {
      if (status != -1) {
        TTF_Quit();
      }
    }

    Sdl2TtfInit(Sdl2TtfInit&) = delete;
  };
}

#endif