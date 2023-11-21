#pragma once

#include "../Colors.h"
#include "Structs.h"

struct Face {
  int a;
  UVCoordinates aUV;
  int b;
  UVCoordinates bUV;
  int c;
  UVCoordinates cUV;

  uint32_t color;

  explicit Face(
      int a = 0, int b = 0, int c = 0, UVCoordinates aUV = {0, 0},
      UVCoordinates bUV = {0, 0}, UVCoordinates cUV = {0, 0},
      uint32_t color = Colors::WHITE
  )
      : a(a), b(b), c(c), aUV(aUV), bUV(bUV), cUV(cUV), color(color) {}
};
