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
  Vec<3> translation = Vec<3>{0, 0, 5};
  // TODO: remove data? what is it for?
  std::vector<std::vector<Vec<3>>> data;

  static Mesh CreateCube() {
    return Mesh(
        {.vertices{
             Vec<3>{-1, -1, -1},  // 1
             Vec<3>{-1, 1, -1},   // 2
             Vec<3>{1, 1, -1},    // 3
             Vec<3>{1, -1, -1},   // 4
             Vec<3>{1, 1, 1},     // 5
             Vec<3>{1, -1, 1},    // 6
             Vec<3>{-1, 1, 1},    // 7
             Vec<3>{-1, -1, 1}    // 8
         },
         .faces{{// front
                 Face{
                     1,
                     2,
                     3,
                     UVCoordinates{0, 1},
                     UVCoordinates{0, 0},
                     UVCoordinates{1, 0},
                     0xFFFFFFFF},
                 Face{
                     1,
                     3,
                     4,
                     UVCoordinates{0, 1},
                     UVCoordinates{1, 0},
                     UVCoordinates{1, 1},
                     0xFFFFFFFF},
                 // right
                 Face{
                     4,
                     3,
                     5,
                     UVCoordinates{0, 1},
                     UVCoordinates{0, 0},
                     UVCoordinates{1, 0},
                     0xFFFFFFFF},
                 Face{
                     4,
                     5,
                     6,
                     UVCoordinates{0, 1},
                     UVCoordinates{1, 0},
                     UVCoordinates{1, 1},
                     0xFFFFFFFF},
                 // back
                 Face{
                     6,
                     5,
                     7,
                     UVCoordinates{0, 1},
                     UVCoordinates{0, 0},
                     UVCoordinates{1, 0},
                     0xFFFFFFFF},
                 Face{
                     6,
                     7,
                     8,
                     UVCoordinates{0, 1},
                     UVCoordinates{1, 0},
                     UVCoordinates{1, 1},
                     0xFFFFFFFF},
                 // left
                 Face{
                     8,
                     7,
                     2,
                     UVCoordinates{0, 1},
                     UVCoordinates{0, 0},
                     UVCoordinates{1, 0},
                     0xFFFFFFFF},
                 Face{
                     8,
                     2,
                     1,
                     UVCoordinates{0, 1},
                     UVCoordinates{1, 0},
                     UVCoordinates{1, 1},
                     0xFFFFFFFF},
                 // top
                 Face{
                     2,
                     7,
                     5,
                     UVCoordinates{0, 1},
                     UVCoordinates{0, 0},
                     UVCoordinates{1, 0},
                     0xFFFFFFFF},
                 Face{
                     2,
                     5,
                     3,
                     UVCoordinates{0, 1},
                     UVCoordinates{1, 0},
                     UVCoordinates{1, 1},
                     0xFFFFFFFF},
                 // bottom
                 Face{
                     6,
                     8,
                     1,
                     UVCoordinates{0, 1},
                     UVCoordinates{0, 0},
                     UVCoordinates{1, 0},
                     0xFFFFFFFF},
                 Face{
                     6,
                     1,
                     4,
                     UVCoordinates{0, 1},
                     UVCoordinates{1, 0},
                     UVCoordinates{1, 1},
                     0xFFFFFFFF}}}}
    );
  }

  void rotate() {
    rotation.x += 0.1;
    rotation.y += 0.1;
    rotation.z += 0.1;
  }
};
