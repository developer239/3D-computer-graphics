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
    int vertexIndices[3];
    int textureIndices[3];
    int normalIndices[3];

    sscanf(
        line.c_str(),
        "f %d/%d/%d %d/%d/%d %d/%d/%d",
        &vertexIndices[0],
        &textureIndices[0],
        &normalIndices[0],
        &vertexIndices[1],
        &textureIndices[1],
        &normalIndices[1],
        &vertexIndices[2],
        &textureIndices[2],
        &normalIndices[2]
    );

    Face face(vertexIndices[0], vertexIndices[1], vertexIndices[2]);
    mesh.faces.push_back(face);
  }
};
