#ifndef GFX_FONT_HPP
#define GFX_FONT_HPP

#include <SDL2/SDL_ttf.h>

namespace gfx {
  class Font {
  public:
    friend void swap(Font &lhs, Font &rhs);

    TTF_Font *data;

    Font(TTF_Font *data = nullptr);
    ~Font();
    Font(Font&) = delete;
    Font(Font &&that);
    Font &operator=(Font &&that);
    
    static Font fromFile(const char *filename, int ptsize);
  };

  void swap(Font &lhs, Font &rhs);
}

#endif