#ifndef GFX_UTIL_HPP
#define GFX_UTIL_HPP

#include <SDL2/SDL.h>

#define GFX_SDL_ERROR() gfxException(__FILE__, __LINE__, "SDL")
#define GFX_IMG_ERROR() gfxException(__FILE__, __LINE__, "IMG")
#define GFX_TTF_ERROR() gfxException(__FILE__, __LINE__, "TTF")
#define GFX_MIX_ERROR() gfxException(__FILE__, __LINE__, "MIX")

void gfxException(const char *file, int lineNumber, const char *sdlModule);

namespace gfx {
  SDL_Rect *updateRect(SDL_Rect &rect, SDL_Rect newValue);
  SDL_Color hslToRgb(float h, float s, float l);
}

#endif