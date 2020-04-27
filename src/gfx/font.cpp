#include <utility>
#include <gfx/util.hpp>
#include <gfx/font.hpp>

gfx::Font::Font(TTF_Font *data):
  data(data) {}

gfx::Font::~Font() {
  if (data != nullptr) {
    TTF_CloseFont(data);
  }
}

gfx::Font::Font(Font &&that) {
  swap(*this, that);
}

gfx::Font &gfx::Font::operator=(Font &&that) {
  swap(*this, that);
  return *this;
}

gfx::Font gfx::Font::fromFile(const char *filename, int ptsize) {
  TTF_Font *ttfFont = TTF_OpenFont(filename, ptsize);

  if (ttfFont == nullptr) {
    GFX_TTF_ERROR();
  }

  return {ttfFont};
}

void gfx::swap(Font &lhs, Font &rhs) {
  using std::swap;
  swap(lhs.data, rhs.data);
}