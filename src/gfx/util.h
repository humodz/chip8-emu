#ifndef GFX_UTIL_H
#define GFX_UTIL_H

#include <cstring>
#include <stdexcept>
#include <SDL2/SDL.h>

#define GFX_SDL_ERROR() gfxException(__FILE__, __LINE__, "SDL", SDL_GetError())
#define GFX_IMG_ERROR() gfxException(__FILE__, __LINE__, "IMG", IMG_GetError())
#define GFX_TTF_ERROR() gfxException(__FILE__, __LINE__, "TTF", TTF_GetError())

void gfxException(const char *file, int lineNumber, const char *sdlModule, const char *sdlError) {
  constexpr int bufSize = 1024 + 64;
  static char buffer[bufSize];
  snprintf(buffer, bufSize, "%s:%d %s %s", file, lineNumber, sdlModule, sdlError);
  throw std::runtime_error(buffer);
}

namespace gfx {

  SDL_Rect *updateRect(SDL_Rect &rect, SDL_Rect newValue) {
    rect = newValue;
    return &rect;
  }

  float hueToRgb(float p, float q, float t) {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
    if (t < 1.f / 2.f) return q;
    if (t < 2.f / 3.f) return p + (q - p) * (2.f/3.f - t) * 6.f;
    return p;
  }

  SDL_Color hslToRgb(float h, float s, float l) {
    if (s == 0) {
      return {0, 0, 0, 255};
    }

    float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    float p = 2 * l - q;
    float r = hueToRgb(p, q, h + 1.f/3.f);
    float g = hueToRgb(p, q, h);
    float b = hueToRgb(p, q, h - 1.f/3.f);

    return {(uint8_t) (r * 255), (uint8_t) (g * 255), (uint8_t) (b * 255), 255};
  }
}

#endif