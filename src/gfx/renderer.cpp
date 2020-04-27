#include <utility>
#include <gfx/renderer.hpp>
#include <gfx/util.hpp>

gfx::Renderer::Renderer(SDL_Renderer *data):
  data(data) {}

gfx::Renderer::Renderer(SDL_Window *window, int index, Uint32 flags) {
  data = SDL_CreateRenderer(window, index, flags);
  if (data == nullptr) {
    GFX_SDL_ERROR();
  }
}

gfx::Renderer::~Renderer() {
  if (data != nullptr) {
    SDL_DestroyRenderer(data);
  }
}

gfx::Renderer::Renderer(Renderer &&that) {
  swap(*this, that);
}

gfx::Renderer &gfx::Renderer::operator=(Renderer &&that) {
  swap(*this, that);
  return *this;
}

void gfx::swap(Renderer &lhs, Renderer &rhs) {
  using std::swap;
  swap(lhs.data, rhs.data);
}