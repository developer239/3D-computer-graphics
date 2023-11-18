#pragma once

#include <vector>
#include "../math/Vec.h"

class Camera {
 public:
  Matrix4x4 projectionMatrix;

  [[deprecated("Use Project3DPointMatrix instead")]] float fovFactor = 640;

  Vec<3> position;

  Camera()
      : position(0, 0, -500),
        fovFactor(640),
        projectionMatrix(Matrix4x4::CreatePerspectiveMatrix(
            M_PI / 3,  // 60 degrees in radians
            800.0f / 600.0f, 0.1f, 100.0f
        )) {}

  [[deprecated("Use Project3DPointMatrix instead")]] Vec<2> Project3DPoint(
      Vec<3> point
  ) const {
    return Vec<2>{
        (fovFactor * point.x) / point.z,
        (fovFactor * point.y) / point.z};
  }

  Vec<4> Project3DPointMatrix(Vec<3> point) const {
    auto result = projectionMatrix * Vec<4>{point.x, point.y, point.z, 1};

    // perform perspective divide with original z-value that is ot stored in w
    if (result.w != 0) {
      result.x /= result.w;
      result.y /= result.w;
      result.z /= result.w;
    }

    return result;
  }
};
