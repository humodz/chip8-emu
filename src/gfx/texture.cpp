#include <utility>
#include <SDL2/SDL_image.h>
#include <gfx/texture.hpp>
#include <gfx/util.hpp>

gfx::Texture::Texture(SDL_Texture *data):
  data(data) {}

gfx::Texture::~Texture() {
  if (data != nullptr) {
    SDL_DestroyTexture(data);
  }
}

gfx::Texture::Texture(Texture &&that) {
  swap(*this, that);
}

gfx::Texture &gfx::Texture::operator=(Texture &&that) {
  swap(*this, that);
  return *this;
}

void gfx::swap(Texture &lhs, Texture &rhs) {
  using std::swap;
  swap(lhs.data, rhs.data);
}

gfx::Texture gfx::Texture::fromFile(SDL_Renderer *renderer, const char *filename) {
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

gfx::Texture gfx::Texture::fromRenderedText(
  SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color textColor
) { 
  SDL_Surface *sdlSurface = TTF_RenderUTF8_Solid(font, text, textColor);

  if (sdlSurface == nullptr) {
    GFX_SDL_ERROR();
  }

  SDL_Texture *sdlTexture = SDL_CreateTextureFromSurface(renderer, sdlSurface);
  SDL_FreeSurface(sdlSurface);

  if (sdlTexture == nullptr) {
    GFX_SDL_ERROR();
  }

  return {sdlTexture};
}