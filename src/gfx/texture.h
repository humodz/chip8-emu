#ifndef TEXTURE_H
#define TEXTURE_H

#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "util.h"

namespace gfx {
  class Texture {
  public:
    friend void swap(Texture &lhs, Texture &rhs);

    SDL_Texture *data;

    Texture(SDL_Texture *data = nullptr):
      data(data) {}

    ~Texture() {
      if (data != nullptr) {
        SDL_DestroyTexture(data);
      }
    }

    Texture &operator=(Texture &&that) {
      swap(*this, that);
      return *this;
    }

    Texture(Texture &&that) {
      swap(*this, that);
    }

    Texture(Texture&) = delete;

    static Texture fromFile(SDL_Renderer *renderer, const char *filename) {
      SDL_Surface *sdlSurface = IMG_Load(filename);

      if (sdlSurface == nullptr) {
        GFX_IMG_ERROR();
      }

      SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(renderer, sdlSurface);
      SDL_FreeSurface(sdlSurface);

      if (sdlTexture == nullptr) {
        GFX_SDL_ERROR();
      }

      return {sdlTexture};
    }

    static Texture fromRenderedText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color textColor) { 
      SDL_Surface *sdlSurface = TTF_RenderUTF8_Solid(font, text, textColor);

      if (sdlSurface == nullptr) {
        GFX_SDL_ERROR();
      }

      SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(renderer, sdlSurface);
      SDL_FreeSurface(sdlSurface);

      if (sdlTexture == nullptr) {
        GFX_SDL_ERROR();
        throw std::runtime_error(SDL_GetError());
      }

      return {sdlTexture};
    }
  };

  void swap(Texture &lhs, Texture &rhs) {
    using std::swap;
    swap(lhs.data, rhs.data);
  }
}

#endif