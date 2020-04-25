
#include <cmath>
#include <array>
#include <sstream>
#include <gfx/gfx.h>
#include <chip8/chip8-emu.h>
#include <chip8/chip8-disassembler.h>
#include <chip8/util.h>
#include <etc/keypad.h>

class EmulatorApp {
public:
  const char *WINDOW_TITLE = "CHIP8";
  int WINDOW_WIDTH = 640;
  int WINDOW_HEIGHT = 640 * 3 / 4;

  gfx::Sdl2Init sdl2Init;
  gfx::Sdl2ImgInit sdl2ImgInit;
  gfx::Sdl2TtfInit sdl2TtfInit;
  gfx::Window window;
  gfx::Renderer renderer;

  gfx::Texture gameScreen;

  EmulatorApp():
    sdl2Init(SDL_INIT_VIDEO),
    sdl2ImgInit(IMG_INIT_PNG & IMG_INIT_JPG),
    sdl2TtfInit(),
    window(
      WINDOW_TITLE,
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT,
      SDL_WINDOW_SHOWN
    ),
    renderer(window.data, -1, SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/),
    gameScreen(
      SDL_CreateTexture(
        renderer.data, SDL_GetWindowPixelFormat(window.data),
        SDL_TEXTUREACCESS_STREAMING,
        chip8::GFX_WIDTH, chip8::GFX_HEIGHT
      )
    ) {}

  void updateGameScreen(chip8::Chip8Emu &emu) {
    Uint32 formatEnum;
    int w;
    int h;
    SDL_QueryTexture(gameScreen.data, &formatEnum, nullptr, &w, &h);
    SDL_PixelFormat *format = SDL_AllocFormat(formatEnum);

    uint8_t *pixels;
    int pitch;
    SDL_LockTexture(gameScreen.data, nullptr, (void **) &pixels, &pitch);

    auto white = SDL_MapRGBA(format, 255, 255, 255, 255);
    auto black = SDL_MapRGBA(format, 0, 0, 0, 0);

    for (int y = emu.lastDrawY; y < emu.lastDrawY + emu.lastDrawH; y++) {
      Uint32 *row = (Uint32 *) (pixels + pitch * y);
      for (int x = emu.lastDrawX; x < emu.lastDrawX + emu.lastDrawW; x++) {
        row[x] = emu.gfx[w * y + x] ? white : black;
      }
    }

    SDL_UnlockTexture(gameScreen.data);
  }

  void renderEmulatorStateText(
    chip8::Chip8Emu &emu, TTF_Font *font, int x, int y
  ) {
    constexpr int textBufSize = 128; 
    char textBuffer[textBufSize] = "";
    std::stringstream ss;
    SDL_Rect textRect;
    gfx::Texture texture;

    snprintf(
      textBuffer, textBufSize,
      "pc=0x%x I=0x%x sp=%d",
      emu.pc, emu.I, emu.sp
    );

    ss << textBuffer << "\n";

    for (int i = 0; i < 16; i++) {
      snprintf(textBuffer, textBufSize, "V%x=0x%02x ", i, emu.V[i]);
      ss << textBuffer;
      if (i % 4 == 3) {
        ss << "\n";
      }
    }

    chip8::printInstruction(emu.opCode, ss);
    ss << "\n";

    ss << "Stack: ";

    if (emu.sp == 0) {
      ss << "(empty)";
    }

    for (int i = 0; i < emu.sp; i++) {
      ss << util::formatHex(emu.stack[i]) << " ";
    }

    ss << " \n";

    int lineOffsetY = 0;

    while (ss.good()) {
      std::string line;
      std::getline(ss, line);
      if (line.size() != 0) {
        texture = gfx::Texture::fromRenderedText(renderer.data, font, line.c_str(), {0, 0, 0, 0});
        SDL_QueryTexture(texture.data, nullptr, nullptr, &textRect.w, &textRect.h);
        textRect.x = x;
        textRect.y = y + textRect.h + lineOffsetY;
        SDL_RenderCopy(renderer.data, texture.data, nullptr, &textRect);
        lineOffsetY += textRect.h;
      }
    }
  }

  int run(int argc, char **argv) {
    (void) argc; (void) argv;

    int FONT_HEIGHT = 12;

    chip8::Chip8Emu emu;
    emu.loadFont(chip8::defaultFont, chip8::defaultFontSize);
    emu.loadProgramFromFile("c8games/TETRIS");

    auto font = gfx::Font::fromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", FONT_HEIGHT);

    etc::Keypad keypad;

    SDL_Rect textRect;
    SDL_Rect viewportRect;

    SDL_Event event;
    bool quit = false;

    Uint32 startTicks = SDL_GetTicks();
    float frames = 1;

    constexpr int fpsTextBufSize = 32;
    char fpsTextBuffer[fpsTextBufSize] = "";

    enum State {
      PAUSED = 1,
      RUNNING = 2,
      STEP = 3,
    };

    State state = PAUSED;

    Uint32 emuTimersCounter = 0;
    Uint32 sinceLastFrameMs = SDL_GetTicks();

    while (!quit) {
      while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
          quit = true;
        } else if (event.type == SDL_KEYUP) {
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE: {
              quit = true;
              break;
            }
            case SDLK_SPACE: {
              state = (state == PAUSED) ? RUNNING : PAUSED;
              break;
            }
            case SDLK_LEFT:
            case SDLK_RIGHT: {
              state = STEP;
              break;
            }
            default: {
              break;
            }
          }
        }
      }

      keypad.update(SDL_GetKeyboardState(nullptr));

      for (int i = 0; i < chip8::NUM_KEYS; i++) {
        emu.key[i] = keypad.isPressed[i];
      }

      // uint16_t nextOp = emu.fetch(emu.pc);

      if (!emu.isRunning()) {
        state = PAUSED;
      }

      if (state != PAUSED) {
        while (emuTimersCounter > 1000 / 60) {
          emuTimersCounter -= 1000 / 60;
          emu.updateTimers();
        }
        Uint32 newSinceLastFrameMs = SDL_GetTicks();
        emuTimersCounter += newSinceLastFrameMs - sinceLastFrameMs;
        sinceLastFrameMs = newSinceLastFrameMs;

        emu.emulateCycle(keypad.keyEvent);   
      }

      if (state == STEP) {
        state = PAUSED;
      }

      if (emu.drawFlag) {
        updateGameScreen(emu);
      }

      Uint32 totalElapsedMs = (SDL_GetTicks() - startTicks);
      snprintf(fpsTextBuffer, fpsTextBufSize, "%.2f FPS", frames * 1000 / totalElapsedMs);
      frames++;

      SDL_SetRenderDrawColor(renderer.data, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderClear(renderer.data);

      int gameScreenHeight = WINDOW_WIDTH * chip8::GFX_HEIGHT / chip8::GFX_WIDTH;
      gfx::updateRect(viewportRect, { 0, 0, WINDOW_WIDTH, gameScreenHeight });
      SDL_RenderSetViewport(renderer.data, &viewportRect);
      SDL_RenderCopy(renderer.data, gameScreen.data, nullptr, nullptr);
      
      SDL_RenderSetViewport(renderer.data, nullptr);

      auto fpsText = gfx::Texture::fromRenderedText(renderer.data, font.data, fpsTextBuffer, {0, 0, 0, 0});
      SDL_QueryTexture(fpsText.data, nullptr, nullptr, &textRect.w, &textRect.h);
      textRect.x = (WINDOW_WIDTH - textRect.w);
      textRect.y = (WINDOW_HEIGHT - textRect.h);
      SDL_RenderCopy(renderer.data, fpsText.data, nullptr, &textRect);

      auto stateText = gfx::Texture::fromRenderedText(renderer.data, font.data, (state == RUNNING) ? "RUNNING" : "PAUSED", {0, 180, 0, 0});
      SDL_QueryTexture(stateText.data, nullptr, nullptr, &textRect.w, &textRect.h);
      textRect.x = (WINDOW_WIDTH - textRect.w);
      textRect.y = (WINDOW_HEIGHT - 2 * textRect.h);
      SDL_RenderCopy(renderer.data, stateText.data, nullptr, &textRect);

      renderEmulatorStateText(emu, font.data, 0, gameScreenHeight);

      int keypadH = 100;
      SDL_Rect keypadRect = { WINDOW_WIDTH - keypadH, gameScreenHeight, keypadH, keypadH};
      keypad.render(renderer.data, &keypadRect);

      SDL_RenderPresent(renderer.data);

      SDL_Delay(1000 / 700);
    }
    return 0;
  }
};

int main(int argc, char **argv) {
  EmulatorApp app;
  return app.run(argc, argv);
}