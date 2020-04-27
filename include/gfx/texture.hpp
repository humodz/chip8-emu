#ifndef GFX_TEXTURE_HPP
#define GFX_TEXTURE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace gfx {
  class Texture {
  public:
    friend void swap(Texture &lhs, Texture &rhs);

    SDL_Texture *data;

    Texture(SDL_Texture *data = nullptr);
    ~Texture();
    Texture(Texture&) = delete;
    Texture(Texture &&that);
    Texture &operator=(Texture &&that);

    static Texture fromFile(SDL_Renderer *renderer, const char *filename);

    static Texture fromRenderedText(
      SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color textColor
    );
  };

  void swap(Texture &lhs, Texture &rhs);
}

#endif