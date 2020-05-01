#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <gfx/sdl-init.hpp>
#include <gfx/util.hpp>

gfx::Sdl2Init::Sdl2Init(Uint32 flags) {
  status = SDL_Init(flags);
  if (status < 0) {
    GFX_SDL_ERROR();
  }
}

gfx::Sdl2Init::~Sdl2Init() {
  if (status >= 0) {
    SDL_Quit();
  }
}

gfx::Sdl2ImgInit::Sdl2ImgInit(int flags) {
  status = IMG_Init(flags) & flags;
  if (status != 0) {
    GFX_IMG_ERROR();
  }
}

gfx::Sdl2ImgInit::~Sdl2ImgInit() {
  if (status == 0) {
    IMG_Quit();
  }
}

gfx::Sdl2TtfInit::Sdl2TtfInit() {
  status = TTF_Init();
  if (status == -1) {
    GFX_TTF_ERROR();
  }
}

gfx::Sdl2TtfInit::~Sdl2TtfInit() {
  if (status != -1) {
    TTF_Quit();
  }
}

gfx::Sdl2MixerInit::Sdl2MixerInit(int frequency, Uint16 format, int channels, int chunkSize) {
  status = Mix_OpenAudio(frequency, format, channels, chunkSize);
  if (status == -1) {
    GFX_MIX_ERROR();
  }
}

gfx::Sdl2MixerInit::~Sdl2MixerInit() {
  if (status != -1) {
    Mix_CloseAudio();
  }
}