#ifndef GFX_FONT_H
#define GFX_FONT_H

#include <algorithm>
#include <SDL2/SDL_ttf.h>
#include "util.h"

namespace gfx {
  class Font {
  public:
    friend void swap(Font &lhs, Font &rhs);

    TTF_Font *data;

    Font(TTF_Font *data):
      data(data) {}

    ~Font() {
      if (data != nullptr) {
        TTF_CloseFont(data);
      }
    }

    Font &operator=(Font &&that) {
      swap(*this, that);
      return *this;
    }

    Font(Font &&that) {
      swap(*this, that);
    }

    Font(Font&) = delete;
    
    static Font fromFile(const char *filename, int ptsize) {
      TTF_Font *ttfFont = TTF_OpenFont(filename, ptsize);

      if (ttfFont == nullptr) {
        GFX_TTF_ERROR();
      }

      return {ttfFont};
    }
  };

  void swap(Font &lhs, Font &rhs) {
    using std::swap;
    swap(lhs.data, rhs.data);
  }
}

#endif