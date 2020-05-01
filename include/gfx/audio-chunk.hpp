#ifndef AUDIO_CHUNK_HPP
#define AUDIO_CHUNK_HPP

#include <SDL2/SDL_mixer.h>

namespace gfx {
  class AudioChunk {
  public:
    Mix_Chunk *data;

    friend void swap(AudioChunk &lhs, AudioChunk &rhs);

    AudioChunk(Mix_Chunk *data = nullptr);
    ~AudioChunk();
    AudioChunk(AudioChunk&) = delete;
    AudioChunk(AudioChunk &&that);
    AudioChunk &operator=(AudioChunk &&that);

    static AudioChunk fromFile(const char *filename);
  };

  void swap(AudioChunk &lhs, AudioChunk &rhs);
}

#endif