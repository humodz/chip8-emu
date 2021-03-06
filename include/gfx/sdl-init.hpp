#ifndef GFX_SDL_INIT_HPP
#define GFX_SDL_INIT_HPP

namespace gfx {
  class Sdl2Init {
  public:
    int status;

    Sdl2Init(Uint32 flags);
    ~Sdl2Init();
    Sdl2Init(Sdl2Init&) = delete;
  };

  class Sdl2ImgInit {
  public:
    int status;

    Sdl2ImgInit(int flags);
    ~Sdl2ImgInit();
    Sdl2ImgInit(Sdl2ImgInit&) = delete;
  };

  class Sdl2TtfInit {
  public:
    int status;

    Sdl2TtfInit();
    ~Sdl2TtfInit();
    Sdl2TtfInit(Sdl2TtfInit&) = delete;
  };

  class Sdl2MixerInit {
  public:
    int status;

    Sdl2MixerInit(int frequency, Uint16 format, int channels, int chunkSize);
    ~Sdl2MixerInit();
    Sdl2MixerInit(Sdl2MixerInit&) = delete;
  };
}

#endif