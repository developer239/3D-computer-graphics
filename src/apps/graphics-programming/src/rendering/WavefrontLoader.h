#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "../geometry/Mesh.h"

class WavefrontLoader {
 public:
  bool LoadObjFile(const std::string& filename, Mesh& mesh) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Could not open file " << filename << std::endl;
      return false;
    }

    std::string line;
    while (std::getline(file, line)) {
      if (line.rfind("v ", 0) == 0) {
        ProcessVertexLine(line, mesh);
      } else if (line.rfind("f ", 0) == 0) {
        ProcessFaceLine(line, mesh);
      }
    }

    return true;
  }

 private:
  void ProcessVertexLine(const std::string& line, Mesh& mesh) {
    std::istringstream iss(line);
    std::string prefix;
    Vec<3> vertex;
    iss >> prefix >> vertex.x >> vertex.y >> vertex.z;
    mesh.vertices.push_back(vertex);
  }

  void ProcessFaceLine(const std::string& line, Mesh& mesh) {
    int vertex_indices[3];
    int texture_indices[3];
    int normal_indices[3];

    sscanf(
        line.c_str(),
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
};
