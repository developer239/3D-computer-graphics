#pragma once

#include <vector>
#include "../math/Vec.h"

class Camera {
 public:
  float fovFactor = 640;

  Vec<3> position;

  Camera() : position(0, 0, -500), fovFactor(640) {}

  Vec<2> Project3DPoint(Vec<3> point) const {
    return Vec<2>{
        (fovFactor * point.x) / point.z,
        (fovFactor * point.y) / point.z};
  }
};
