#include <utility>
#include <gfx/audio-chunk.hpp>
#include <gfx/util.hpp>

gfx::AudioChunk::AudioChunk(Mix_Chunk *data):
  data(data) {}

gfx::AudioChunk::~AudioChunk() {
  if (data != nullptr) {
    Mix_FreeChunk(data);
  }
}

gfx::AudioChunk::AudioChunk(AudioChunk &&that) {
  swap(*this, that);
}

gfx::AudioChunk &gfx::AudioChunk::operator=(AudioChunk &&that) {
  swap(*this, that);
  return *this;
}

void gfx::swap(AudioChunk &lhs, AudioChunk &rhs) {
  using std::swap;
  swap(lhs.data, rhs.data);
}

gfx::AudioChunk gfx::AudioChunk::fromFile(const char *filename) {
  Mix_Chunk *data = Mix_LoadWAV(filename);
  if (data == nullptr) {
    GFX_MIX_ERROR();
  }
  return {data};
}