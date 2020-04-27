#include <utility>
#include <SDL2/SDL_image.h>
#include <gfx/surface.hpp>
#include <gfx/util.hpp>

gfx::Surface::Surface(SDL_Surface *surface):
  data(surface) {}

gfx::Surface::~Surface() {
  if (data != nullptr) {
    SDL_FreeSurface(data);
  }
}

gfx::Surface &gfx::Surface::operator=(Surface &&that) {
  swap(*this, that);
  return *this;
}

gfx::Surface::Surface(Surface &&that) {
  swap(*this, that);
}

gfx::Surface gfx::Surface::fromFile(
  const char *filename, const SDL_PixelFormat *optimizeTo
) {
  SDL_Surface *sdlSurface = IMG_Load(filename);

  if (sdlSurface == nullptr) {
    GFX_IMG_ERROR();
  }

  SDL_Surface *origSurface = sdlSurface;

  if (optimizeTo != nullptr) {
    sdlSurface = SDL_ConvertSurface(sdlSurface, optimizeTo, 0);
    SDL_FreeSurface(origSurface);

    if (sdlSurface == nullptr) {
      GFX_SDL_ERROR();
    }
  }

  return {sdlSurface};
}

void gfx::swap(Surface &lhs, Surface &rhs) {
  using std::swap;
  swap(lhs.data, rhs.data);
}