#pragma once

#include <vector>
#include "../math/Vec.h"

class Camera {
  public:
    Vec<3> position;

    Camera() {
      position = Vec<3>(0, 0, -500);
    }
};

