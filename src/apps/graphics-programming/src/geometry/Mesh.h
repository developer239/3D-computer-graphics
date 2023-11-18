#pragma once

#include <vector>
#include "../math/Faces.h"
#include "../math/Vec.h"

class Mesh {
 public:
  std::vector<Vec<3>> vertices = {};
  std::vector<Face> faces = {};
  Vec<3> rotation = Vec<3>{0, 0, 0};
  Vec<3> scale = Vec<3>{1, 1, 1};
  Vec<3> translation = Vec<3>{0, 0, 0};
  std::vector<std::vector<Vec<3>>> data;

  void rotate() {
    rotation.x += 0.1;
    rotation.y += 0.1;
    rotation.z += 0.1;
  }
};
