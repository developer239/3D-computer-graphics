#pragma once

#include <SDL.h>
#include "vector"

class Texture {
 public:
  int width;
  int height;

  std::vector<uint32_t> colorBuffer;
  std::unique_ptr<SDL_Texture, void (*)(SDL_Texture*)> colorBufferTexture;

  Texture(int width, int height)
      : width(width),
        height(height),
        colorBuffer(width * height),
        colorBufferTexture(nullptr, SDL_DestroyTexture) {}

  uint32_t& operator[](int index) { return colorBuffer[index]; }
};
