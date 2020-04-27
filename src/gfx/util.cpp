#include <cstring>
#include <stdexcept>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <gfx/util.hpp>

namespace {
  float hueToRgb(float p, float q, float t);
}

void gfxException(const char *file, int lineNumber, const char *sdlModule) {
  const char *sdlError = "gfxException: Invalid SDL module";

  if (strcmp(sdlModule, "SDL") == 0) {
    sdlError = SDL_GetError();
  } else if (strcmp(sdlModule, "IMG") == 0) {
    sdlError = IMG_GetError();
  } else if (strcmp(sdlModule, "TTF") == 0) {
    sdlError = TTF_GetError();
  }

  constexpr int bufSize = 1024 + 128;
  static char buffer[bufSize];
  snprintf(buffer, bufSize, "%s:%d %s %s", file, lineNumber, sdlModule, sdlError);
  throw std::runtime_error(buffer);
}

SDL_Rect *gfx::updateRect(SDL_Rect &rect, SDL_Rect newValue) {
  rect = newValue;
  return &rect;
}

SDL_Color gfx::hslToRgb(float h, float s, float l) {
  if (s == 0) {
    return {0, 0, 0, 255};
  }

  float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
  float p = 2 * l - q;
  float r = hueToRgb(p, q, h + 1.f/3.f);
  float g = hueToRgb(p, q, h);
  float b = hueToRgb(p, q, h - 1.f/3.f);

  return {(uint8_t) (r * 255), (uint8_t) (g * 255), (uint8_t) (b * 255), 255};
}

namespace {
  float hueToRgb(float p, float q, float t) {
    if (t < 0.f) t += 1;
    if (t > 1.f) t -= 1;
    if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
    if (t < 1.f / 2.f) return q;
    if (t < 2.f / 3.f) return p + (q - p) * (2.f/3.f - t) * 6.f;
    return p;
  }
}