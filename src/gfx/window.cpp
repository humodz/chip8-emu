#include <utility>
#include <gfx/util.hpp>
#include <gfx/window.hpp>

gfx::Window::Window(const char* title, int x, int y, int w, int h, Uint32 flags) {
  data = SDL_CreateWindow(title, x, y, w, h, flags);
  if (data == nullptr) {
    GFX_SDL_ERROR();
  }
}

gfx::Window::~Window() {
  if (data != nullptr) {
    SDL_DestroyWindow(data);
  }
}

gfx::Window::Window(Window &&that) {
  swap(*this, that);
}

gfx::Window &gfx::Window::operator=(gfx::Window &&that) {
  swap(*this, that);
  return *this;
}

SDL_Surface *gfx::Window::surface() const {
  auto *surface = SDL_GetWindowSurface(data);
  if (surface == nullptr) {
    GFX_SDL_ERROR();
  }
  return surface;
}

SDL_PixelFormat *gfx::Window::pixelFormat() const {
  auto formatEnum = SDL_GetWindowPixelFormat(data);

  if (formatEnum == SDL_PIXELFORMAT_UNKNOWN) {
    GFX_SDL_ERROR();
  }

  auto *format = SDL_AllocFormat(SDL_GetWindowPixelFormat(data));

  if (format == nullptr) {
    GFX_SDL_ERROR();
  }

  return format;
}

void gfx::swap(Window &lhs, Window &rhs) {
  using std::swap;
  swap(lhs.data, rhs.data);
}