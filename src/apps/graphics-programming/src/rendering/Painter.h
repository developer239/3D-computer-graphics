#pragma once

#include <SDL.h>
#include <cmath>
#include <cstdint>
#include <memory>
#include <vector>
#include "../Colors.h"
#include "../geometry/Mesh.h"
#include "../math/Matrix4x4.h"
#include "../math/Triangle.h"
#include "../utils.h"
#include "Camera.h"
#include "ColorBuffer.h"

class Painter {
 public:
  ColorBuffer colorBuffer;

  Painter(int windowWidth, int windowHeight)
      : colorBuffer(windowWidth, windowHeight) {}

  void Pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < colorBuffer.windowWidth && y >= 0 &&
        y < colorBuffer.windowHeight) {
      colorBuffer[y * colorBuffer.windowWidth + x] = color;
    }
  }

  void Line(int x0, int y0, int x1, int y1, uint32_t color = Colors::MAGENTA) {
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    int sideLength = std::max(abs(deltaX), abs(deltaY));

    float xIncrement = deltaX / (float)sideLength;
    float yIncrement = deltaY / (float)sideLength;

    float currentX = x0;
    float currentY = y0;

    for (int i = 0; i <= sideLength; i++) {
      Pixel(std::round(currentX), std::round(currentY), color);
      currentX += xIncrement;
      currentY += yIncrement;
    }
  }

  void Rectangle(
      int x, int y, int width, int height, uint32_t color = Colors::YELLOW
  ) {
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        int current_x = x + i;
        int current_y = y + j;

        Pixel(current_x, current_y, color);
      }
    }
  }

  void Triangle(
      int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color
  ) {
    Line(x0, y0, x1, y1, color);
    Line(x1, y1, x2, y2, color);
    Line(x2, y2, x0, y0, color);
  }

  void FilledTriangle(
      int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color
  ) {
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1) {
      intSwap(&y0, &y1);
      intSwap(&x0, &x1);
    }
    if (y1 > y2) {
      intSwap(&y1, &y2);
      intSwap(&x1, &x2);
    }
    if (y0 > y1) {
      intSwap(&y0, &y1);
      intSwap(&x0, &x1);
    }

    if (y1 == y2) {
      // Draw flat-bottom triangle
      fillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
      // Draw flat-top triangle
      fillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
    } else {
      // Calculate the new vertex (Mx,My) using triangle similarity
      int My = y1;
      int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

      // Draw flat-bottom triangle
      fillFlatBottomTriangle(x0, y0, x1, y1, Mx, My, color);

      // Draw flat-top triangle
      fillFlatTopTriangle(x1, y1, Mx, My, x2, y2, color);
    }
  }

  void Mesh(Mesh mesh, bool shouldCull, Camera camera) {
    std::vector<struct Triangle> trianglesToRender;

    auto mat4x4Scale = Matrix4x4::CreateScaleMatrix(mesh.scale.x, mesh.scale.y, mesh.scale.z);

    // Loop all triangle faces of our mesh
    for (const auto& meshFace : mesh.faces) {
      std::vector<Vec<3>> faceVertices(3);

      faceVertices[0] = mesh.vertices[meshFace.a - 1];
      faceVertices[1] = mesh.vertices[meshFace.b - 1];
      faceVertices[2] = mesh.vertices[meshFace.c - 1];

      std::vector<Vec<3>> transformedVertices(3);

      // Loop all three vertices of this current face and apply transformations
      for (int j = 0; j < 3; j++) {
        Vec<4> vertex4D = Vec<4>(faceVertices[j], 1);
        Vec<4> transformed_vertex4D = mat4x4Scale * vertex4D;
        Vec<3> transformed_vertex = transformed_vertex4D.ToVec3();

        // Translate the vertices away from the camera
        transformed_vertex.z += 5;

        // Save transformed vertex in the array of transformed vertices
        transformedVertices[j] = transformed_vertex;
      }

      if (shouldCull) {
        // Check backface culling
        Vec<3> vector_a = transformedVertices[0]; /*   A   */
        Vec<3> vector_b = transformedVertices[1]; /*  / \  */
        Vec<3> vector_c = transformedVertices[2]; /* C---B */

        // Get the vector subtraction of B-A and C-A
        Vec<3> vector_ab = vector_b - vector_a;
        Vec<3> vector_ac = vector_c - vector_a;

        // Compute the face normal (using cross product to find perpendicular)
        Vec<3> normal = vector_ab.CrossProduct(vector_ac);

        // Find the vector between a point in the triangle and the camera origin
        Vec<3> camera_ray = camera.position - vector_a;

        float dot_normal_camera =
            normal.Normalize().DotProduct(camera_ray.Normalize());

        // Bypass the triangles that are looking away from the camera
        if (dot_normal_camera < 0) {
          continue;
        }
      }

      struct Triangle projected_triangle;

      // Loop all three vertices to perform projection
      for (int j = 0; j < 3; j++) {
        // Project the current vertex
        Vec<2> projected_point = camera.Project3DPoint(transformedVertices[j]);

        // Scale and translate the projected points to the middle of the screen
        projected_point.x += (colorBuffer.windowWidth / 2);
        projected_point.y += (colorBuffer.windowHeight / 2);

        if (j == 0) {
          projected_triangle.p1 = projected_point;
        } else if (j == 1) {
          projected_triangle.p2 = projected_point;
        } else if (j == 2) {
          projected_triangle.p3 = projected_point;
        }
      }

      projected_triangle.averageDepth =
          (transformedVertices[0].z + transformedVertices[1].z +
           transformedVertices[2].z) /
          3;

      // Save the projected triangle in the array of triangles to render
      trianglesToRender.push_back(projected_triangle);

      // Painter algorithm - sort the triangles by average depth
      std::sort(
          trianglesToRender.begin(),
          trianglesToRender.end(),
          [](const auto& a, const auto& b) {
            return a.averageDepth > b.averageDepth;
          }
      );
    }

    int num_triangles = trianglesToRender.size();
    for (const auto& triangle : trianglesToRender) {
      // Draw unfilled triangle
      FilledTriangle(
          triangle.p1.x,
          triangle.p1.y,  // vertex A
          triangle.p2.x,
          triangle.p2.y,  // vertex B
          triangle.p3.x,
          triangle.p3.y,  // vertex C
          Colors::GREEN
      );
      Triangle(
          triangle.p1.x,
          triangle.p1.y,  // vertex A
          triangle.p2.x,
          triangle.p2.y,  // vertex B
          triangle.p3.x,
          triangle.p3.y,  // vertex C
          Colors::BLACK
      );
    }
  }

  void BackgroundGrid() {
    for (int y = 0; y < colorBuffer.windowHeight; y += 10) {
      for (int x = 0; x < colorBuffer.windowWidth; x += 10) {
        if (x % 10 == 0 || y % 10 == 0) {
          Pixel(x, y, Colors::WHITE);
        }
      }
    }
  }

 private:
  void fillFlatBottomTriangle(
      int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color
  ) {
    // Find the two slopes (two triangle legs)
    float inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
    float inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

    // Start x_start and x_end from the top vertex (x0,y0)
    float x_start = x0;
    float x_end = x0;

    // Loop all the scanlines from top to bottom
    for (int y = y0; y <= y2; y++) {
      Line(x_start, y, x_end, y, color);
      x_start += inv_slope_1;
      x_end += inv_slope_2;
    }
  }

  void fillFlatTopTriangle(
      int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color
  ) {
    // Find the two slopes (two triangle legs)
    float inv_slope_1 = (float)(x2 - x0) / (y2 - y0);
    float inv_slope_2 = (float)(x2 - x1) / (y2 - y1);

    // Start x_start and x_end from the bottom vertex (x2,y2)
    float x_start = x2;
    float x_end = x2;

    // Loop all the scanlines from bottom to top
    for (int y = y2; y >= y0; y--) {
      Line(x_start, y, x_end, y, color);
      x_start -= inv_slope_1;
      x_end -= inv_slope_2;
    }
  }
};
