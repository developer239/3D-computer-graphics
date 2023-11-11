#pragma once

#include <vector>
#include "Camera.h"

class Projector {
 public:
  float fovFactor = 640;

  Camera camera;

  Projector() { camera = Camera(); }

  Vec<2> Project3DPoint(Vec<3> point) const {
    return Vec<2>{
        (fovFactor * point.x) / point.z,
        (fovFactor * point.y) / point.z};
  }
};
