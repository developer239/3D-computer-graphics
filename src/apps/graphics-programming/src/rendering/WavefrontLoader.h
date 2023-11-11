#pragma once

#include <cstdio>
#include <iostream>
#include "../geometry/Mesh.h"

class WavefrontLoader {
 public:
  static bool LoadObjFile(char* filename, Mesh& mesh) {
    FILE* file = fopen(filename, "r");
    if (file == nullptr) {
      printf("Could not open file %s\n", filename);
      return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
      if (strncmp(line, "v ", 2) == 0) {
        Vec<3> vertex;
        sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);

        mesh.vertices.push_back(vertex);
      }

      if (strncmp(line, "f ", 2) == 0) {
        int vertex_indices[3];
        int texture_indices[3];
        int normal_indices[3];

        sscanf(
            line,
            "f %d/%d/%d %d/%d/%d %d/%d/%d",
            &vertex_indices[0],
            &texture_indices[0],
            &normal_indices[0],
            &vertex_indices[1],
            &texture_indices[1],
            &normal_indices[1],
            &vertex_indices[2],
            &texture_indices[2],
            &normal_indices[2]
        );
        Face face(vertex_indices[0], vertex_indices[1], vertex_indices[2]);

        mesh.faces.push_back(face);
      }
    }

    fclose(file);
    return true;
  }
};
