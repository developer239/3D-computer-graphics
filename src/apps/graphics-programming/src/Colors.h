#pragma once

#include <cstdint>

namespace Colors {

constexpr uint32_t BLACK = 0xFF000000;
constexpr uint32_t MAGENTA = 0xFFFF00FF;
constexpr uint32_t YELLOW = 0xFFFFFF00;
constexpr uint32_t GREEN = 0xFF00FF00;
constexpr uint32_t WHITE = 0xFFFFFFFF;

uint32_t Darken(uint32_t color, double percentageFactor) {
  if(percentageFactor < 0) {
    percentageFactor = 0;
  }
  if(percentageFactor > 1) {
    percentageFactor = 1;
  }

  uint32_t a = (color & 0xFF000000);
  uint32_t r = (color & 0x00FF0000) * percentageFactor;
  uint32_t g = (color & 0x0000FF00) * percentageFactor;
  uint32_t b = (color & 0x000000FF) * percentageFactor;

  uint32_t newColor = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

  return newColor;
}

}  // namespace Colors
