#include <cstdlib>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
#include <chip8/chip8-emu.h>
#include <chip8/chip8-disassembler.h>
#include <gfx/surface.h>

#define ASSERT_SDL(cond) assertSdl(cond, #cond)

void assertSdl(bool cond, const char* what) {
	if (!cond) {
    std::cerr << "Assertion failed: \"" << what << "\", SDL error: " << SDL_GetError() << std::endl;
		exit(1);
	}
}

SDL_Rect makeRect(int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

SDL_Rect *makeRect(SDL_Rect &rect, int x, int y, int w, int h) {
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return &rect;
}

class GameScreen {
public:
  std::vector<Uint32> pixels;
  gfx::Surface surface;

  GameScreen(GameScreen &that) {
    int width = that.surface.data->w;
    int height = that.surface.data->h;
    pixels = that.pixels;
    
    int depth = sizeof(Uint32) * 8;
    int pitch = sizeof(Uint32) * width;

    surface = SDL_CreateRGBSurfaceWithFormatFrom(
      pixels.data(), width, height,
      depth, pitch, SDL_PIXELFORMAT_RGBA8888
    );
  }

  GameScreen(int width, int height) {
    pixels.resize(width * height, 0);
    
    int depth = sizeof(Uint32) * 8;
    int pitch = sizeof(Uint32) * width;

    surface = SDL_CreateRGBSurfaceWithFormatFrom(
      pixels.data(), width, height,
      depth, pitch, SDL_PIXELFORMAT_RGBA8888
    );
  }

  void update(const uint8_t *gfx, const SDL_Rect &rect) {
    int width = surface.data->w;
    int height = surface.data->h;

    for (int x = rect.x; x < rect.x + rect.w; x++) {
      for (int y = rect.y; y < rect.y + rect.h; y++) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
          pixels[width * y + x] = gfx[width * y + x] ? 0xFFFFFF00 : 0x0;
        }
      }
    }
  }
};

void drawKeypad(bool keys[16], SDL_Surface *dst, SDL_Rect *dstRect) {
  (void) keys;

  auto red = SDL_MapRGB(dst->format, 0xFF, 0x00, 0x00);
  auto gray = SDL_MapRGB(dst->format, 0xAA, 0xAA, 0xAA);

  bool mappedKeys[] = {
    keys[0x1], keys[0x2], keys[0x3], keys[0xC],
    keys[0x4], keys[0x5], keys[0x6], keys[0xD],
    keys[0x7], keys[0x8], keys[0x9], keys[0xE],
    keys[0xA], keys[0x0], keys[0xB], keys[0xF],
  };

  SDL_Rect rect;
  auto buttonW = dstRect->w / 4;
  auto buttonH = dstRect->h / 4;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      auto color = mappedKeys[i * 4 + j] ? red : gray;
      makeRect(rect, dstRect->x + buttonW * j, dstRect->y + buttonH * i, buttonW, buttonH);
      SDL_FillRect(dst, &rect, color);
    }
  }
}

int main(int argc, const char **argv) {
  (void) argc;
  (void) argv;

  chip8::Chip8Emu emu;
  emu.loadProgramFromFile("c8games/KEYPAD");
  emu.loadFont(chip8::defaultFont, chip8::defaultFontSize);

	ASSERT_SDL(SDL_Init(SDL_INIT_VIDEO) >= 0);

  const int SCREEN_WIDTH = 640;
  const int SCREEN_HEIGHT = 480;

  auto window = SDL_CreateWindow(
    "CHIP8 Emulator",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );
	ASSERT_SDL(window != nullptr);

  auto screenSurface = SDL_GetWindowSurface(window);

  GameScreen gameScreen(chip8::GFX_WIDTH, chip8::GFX_HEIGHT);

  bool firstDrawDone = false;
  bool quit = false;
  SDL_Event event;
  SDL_Rect rect;

  auto white = SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF);

  // 1 2 3 C
  // 4 5 6 E
  // 7 8 9 D
  // A 0 B F
  int keyCodes[] = {
    SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
    SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V,
  };

  // bool keys[16];
  // (void) keys;

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = true;
        break;
      }
    }

    if (quit) {
      break;
    }

    bool changed = false;
    auto keyboard = SDL_GetKeyboardState(nullptr);

    for (int i = 0; i < 16; i++) {
      if (emu.key[i] != keyboard[keyCodes[i]]) {
        changed = true;
      }
      emu.key[i] = keyboard[keyCodes[i]];
    }

    if (emu.state == chip8::EmulationState::RUNNING) {
      emu.emulateCycle(std::cout);
    }

    if (emu.state == chip8::EmulationState::STOPPED_UNRECOGNIZED_OPCODE) {
      auto instr = (emu.pc - chip8::PROGRAM_OFFSET) / 2 + 1;
      std::cout << "addr = " << instr << std::endl;
      emu.state = chip8::EmulationState::STOPPED;
    }

    if (emu.drawFlag) {
      makeRect(rect, emu.lastDrawX, emu.lastDrawY, emu.lastDrawW, emu.lastDrawH);
      gameScreen.update(emu.gfx, rect);
    }

    if (!firstDrawDone || emu.drawFlag || changed) {
      firstDrawDone = true;
      SDL_FillRect(screenSurface, nullptr, white);

      auto gameScreenHeight = SCREEN_WIDTH / chip8::GFX_WIDTH * chip8::GFX_HEIGHT;
      makeRect(rect, 0, 0, SCREEN_WIDTH, gameScreenHeight);
      SDL_BlitScaled(gameScreen.surface.data, nullptr, screenSurface, &rect);

      auto keypadHeight = SCREEN_HEIGHT - gameScreenHeight;
      makeRect(rect, 0, gameScreenHeight, keypadHeight, keypadHeight);
      drawKeypad(emu.key, screenSurface, &rect);

      SDL_UpdateWindowSurface(window);
    }
    SDL_Delay(1000 / 500);
  }

  SDL_Quit();
  return 0;
}

int mainOld(int argc, const char **argv) {
  (void) argc;
  (void) argv;

  std::ios_base::sync_with_stdio(false);

  chip8::Chip8Emu emu;
  emu.loadProgramFromFile("c8games/MAZE");

  while (emu.state == chip8::EmulationState::RUNNING) {
    emu.emulateCycle(std::cerr);

    if (emu.drawFlag) {
      emu.drawScreen(std::cout);
    }
  }

  return 0;
}
