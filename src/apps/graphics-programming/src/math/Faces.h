#pragma once

#include "../Colors.h"
#include "Structs.h"

struct Face {
  int a;
  int b;
  int c;
  UVCoordinates a_uv;
  UVCoordinates b_uv;
  UVCoordinates c_uv;
  uint32_t color;

  explicit Face(
      int a = 0, int b = 0, int c = 0, UVCoordinates a_uv = {0, 0},
      UVCoordinates b_uv = {0, 0}, UVCoordinates c_uv = {0, 0},
      uint32_t color = Colors::WHITE
  )
      : a(a), b(b), c(c), a_uv(a_uv), b_uv(b_uv), c_uv(c_uv), color(color) {}
};
