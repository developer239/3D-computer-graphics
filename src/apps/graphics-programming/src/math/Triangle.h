#pragma once

#include <vector>
#include "Vec.h"

struct Triangle {
 public:
  Vec<4> p1;
  Vec<4> p2;
  Vec<4> p3;

  // TODO: this seems to be duplicated? Why have it both here and in p1, p2, p3?
  UVCoordinates p1_uv;
  UVCoordinates p2_uv;
  UVCoordinates p3_uv;

  double averageDepth;
  double lightIntensityFactor;

  explicit Triangle(
      Vec<4> p1 = Vec<4>(), Vec<4> p2 = Vec<4>(), Vec<4> p3 = Vec<4>(),
      double averageDepth = 0
  )
      : p1(p1), p2(p2), p3(p3), averageDepth(averageDepth) {}
};
