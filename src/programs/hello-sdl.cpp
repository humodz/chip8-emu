#include <cstdio>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <gfx/gfx.hpp>

int main() {
  int WINDOW_WIDTH = 400;
  int WINDOW_HEIGHT = 300;

  gfx::Sdl2Init sdl2Init(SDL_INIT_VIDEO);
  gfx::Sdl2ImgInit sdl2ImgInit(IMG_INIT_PNG & IMG_INIT_JPG);
  gfx::Sdl2TtfInit sdl2TtfInit;
  gfx::Sdl2MixerInit sdl2MixerInit(44100, MIX_DEFAULT_FORMAT, 2, 2048);

  gfx::Window window(
    "Hello SDL",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH, WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN
  );

  gfx::Renderer renderer(window.data, -1, SDL_RENDERER_ACCELERATED);

  auto font = gfx::Font::fromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", 16);
  auto picture = gfx::Texture::fromFile(renderer.data, "assets/shore.jpg");
  auto soundEffect = gfx::AudioChunk::fromFile("assets/blip.wav");

  SDL_SetRenderDrawColor(renderer.data, 0xFF, 0xFF, 0xFF, 0xFF);

  SDL_Rect rect;

  SDL_Event event;
  bool quit = false;

  auto startTicks = SDL_GetTicks();
  float frames = 1;

  constexpr int textBufSize = 32;
  char textBuffer[textBufSize] = "";

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (event.type == SDL_KEYDOWN) {
        auto key = event.key.keysym.sym;
        if (key == SDLK_ESCAPE) {
          quit = true;
        } else if (key == SDLK_SPACE) {
          Mix_PlayChannel(-1, soundEffect.data, 0);
        }
      }
    }

    float elapsedSecs = (SDL_GetTicks() - startTicks) / 1000.0;
    snprintf(textBuffer, textBufSize, "%.2f FPS", frames / elapsedSecs);
    frames++;

    auto text = gfx::Texture::fromRenderedText(renderer.data, font.data, textBuffer, {0, 0, 0, 0});
    SDL_QueryTexture(text.data, nullptr, nullptr, &rect.w, &rect.h);
    // rect.x = WINDOW_WIDTH - rect.w;
    // rect.y = WINDOW_HEIGHT - rect.h;
    rect.x = (WINDOW_WIDTH - rect.w) / 2;
    rect.y = (WINDOW_HEIGHT - rect.h) / 2; 

    SDL_RenderClear(renderer.data);
    SDL_RenderCopy(renderer.data, picture.data, nullptr, nullptr);
    SDL_RenderCopy(renderer.data, text.data, nullptr, &rect);
    SDL_RenderPresent(renderer.data);

    // SDL_Delay(1000 / 20);
  }

  return 0;
}