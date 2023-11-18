#pragma once

#include <SDL.h>
#include "core/Renderer.h"
#include "vector"

class ColorBuffer {
 public:
  int windowWidth;
  int windowHeight;

  std::vector<uint32_t> colorBuffer;
  std::unique_ptr<SDL_Texture, void (*)(SDL_Texture*)> colorBufferTexture;

  ColorBuffer(int windowWidth, int windowHeight)
      : windowWidth(windowWidth),
        windowHeight(windowHeight),
        colorBuffer(windowWidth * windowHeight),
        colorBufferTexture(nullptr, SDL_DestroyTexture) {}

  void RenderColorBuffer(Core::Renderer& renderer) {
    if (!colorBufferTexture) {
      colorBufferTexture = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture*)>(
          SDL_CreateTexture(
              renderer.Get().get(),
              SDL_PIXELFORMAT_ARGB8888,
              SDL_TEXTUREACCESS_STREAMING,
              windowWidth,
              windowHeight
          ),
          SDL_DestroyTexture
      );
    }

    SDL_UpdateTexture(
        colorBufferTexture.get(),
        nullptr,
        colorBuffer.data(),
        windowWidth * sizeof(uint32_t)
    );

    SDL_RenderCopy(
        renderer.Get().get(),
        colorBufferTexture.get(),
        nullptr,
        nullptr
    );
  }

  void ClearColorBuffer(uint32_t color = Colors::BLACK) {
    std::fill(colorBuffer.begin(), colorBuffer.end(), color);
  }

  uint32_t& operator[](int index) { return colorBuffer[index]; }
};
