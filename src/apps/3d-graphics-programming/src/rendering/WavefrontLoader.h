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

    std::vector<UVCoordinates> UVs = {};
    std::string line;
    while (std::getline(file, line)) {
      if (line.rfind("v ", 0) == 0) {
        ProcessVertexLine(line, mesh);
      }

      if (line.rfind("f ", 0) == 0) {
        ProcessFaceLine(line, mesh, UVs);
      }

      if (line.rfind("vt ", 0) == 0) {
        ProcessTextureLine(line, mesh, UVs);
      }
    }

    return true;
  }

 private:
  void ProcessTextureLine(
      const std::string& line, Mesh& mesh, std::vector<UVCoordinates>& UVs
  ) {
    UVCoordinates UVcoords;

    std::istringstream iss(line);
    sscanf(line.c_str(), "vt %f %f", &UVcoords.u, &UVcoords.v);

    UVs.push_back(UVcoords);
  }

  void ProcessVertexLine(const std::string& line, Mesh& mesh) {
    std::istringstream iss(line);
    std::string prefix;
    Vec<3> vertex;
    iss >> prefix >> vertex.x >> vertex.y >> vertex.z;
    mesh.vertices.push_back(vertex);
  }

  void ProcessFaceLine(
      const std::string& line, Mesh& mesh, std::vector<UVCoordinates>& UVs
  ) {
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

    Face face(
        vertexIndices[0] - 1,
        vertexIndices[1] - 1,
        vertexIndices[2] - 1,
        UVs[textureIndices[0] - 1],
        UVs[textureIndices[1] - 1],
        UVs[textureIndices[2] - 1]
    );
    mesh.faces.push_back(face);
  }
};
