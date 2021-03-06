#ifndef KEYPAD_HPP
#define KEYPAD_HPP

#include <algorithm>
#include <array>
#include <SDL2/SDL.h>
#include <gfx/gfx.hpp>

class Keypad {
public:
  int keyEvent = -1;
  std::array<bool, 16> isPressed;
  std::array<int, 16> keyMappings = {
    SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
    SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
  };

  std::array<int, 16> keypadPositionMappings = {
    0x1, 0x2, 0x3, 0xC,
    0x4, 0x5, 0x6, 0xD,
    0x7, 0x8, 0x9, 0xE,
    0xA, 0x0, 0xB, 0xF
  };

  SDL_Color colors[2][2] = {
    { // not pressed
      // not dark
      { 0xC0, 0xC0, 0xC0, 0xFF },
      // dark
      { 0x80, 0x80, 0x80, 0xFF },
    },
    { // pressed
      // not dark
      { 0xFF, 0x50, 0x50, 0xFF },
      // dark
      { 0xFF, 0x00, 0x00, 0xFF },
    },
  };

  Keypad() {
    std::fill(isPressed.begin(), isPressed.end(), false);
  }

  void update(const uint8_t *sdlKeyboardState) {
    keyEvent = -1;
    for (int i = 0; i < 16; i++) {
      bool newIsPressed = sdlKeyboardState[keyMappings.at(i)];

      if (keyEvent == -1 && newIsPressed && !isPressed.at(i)) {
        keyEvent = i;
      }

      isPressed[i] = newIsPressed;
    }
  }

  void render(SDL_Renderer *renderer, SDL_Rect *dstRect) {
    SDL_Rect dst;

    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        dst.x = dstRect->x + dstRect->w * (x / 4.f);
        dst.y = dstRect->y + dstRect->h * (y / 4.f);

        int nextX = dstRect->x + dstRect->w * ((x + 1) / 4.f);
        int nextY = dstRect->y + dstRect->h * ((y + 1) / 4.f);

        dst.w = nextX - dst.x + 1;
        dst.h = nextY - dst.y + 1;

        bool pressed = isPressed.at(keypadPositionMappings.at(x + y * 4));
        bool darker = (x + y) % 2 == 1;

        SDL_Color *color = &colors[pressed][darker];

        SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 0xFF);
        SDL_RenderFillRect(renderer, &dst);
      }
    }
  }
};

#endif