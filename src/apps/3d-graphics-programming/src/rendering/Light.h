#pragma once

#include "../geometry/Mesh.h"
#include "../math/Triangle.h"
#include "../math/Vec.h"
#include "Camera.h"

class Light {
 public:
  Vec<3> position;
  Vec<3> direction;

  Light(Vec<3> position = Vec<3>(0, 0, 0), Vec<3> direction = Vec<3>(0, 0, 1))
      : position(position), direction(direction) {}
};
