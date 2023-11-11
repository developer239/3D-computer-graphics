#pragma once

#include <vector>
#include "Vec.h"

struct Triangle {
 public:
  Vec<2> p1;
  Vec<2> p2;
  Vec<2> p3;
  double averageDepth;

  explicit Triangle(
      Vec<2> p1 = Vec<2>(), Vec<2> p2 = Vec<2>(), Vec<2> p3 = Vec<2>(),
      double averageDepth = 0
  )
      : p1(p1), p2(p2), p3(p3), averageDepth(averageDepth) {}
};
