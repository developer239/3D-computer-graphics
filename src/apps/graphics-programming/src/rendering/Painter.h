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
#include "Light.h"

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

  // Draw textured pixel at position x and y using interpolation
  void Texel(
      int x, int y, Vec<4> pointA, Vec<4> pointB, Vec<4> pointC,
      UVCoordinates auv, UVCoordinates buv, UVCoordinates cuv,
      const std::unique_ptr<uint32_t[]>& texture, int textureWidth = 64,
      int textureHeight = 64
  ) {
    auto pointP = Vec<2>{(float)x, (float)y};
    Vec<2> a = pointA.ToVec2();
    Vec<2> b = pointB.ToVec2();
    Vec<2> c = pointC.ToVec2();

    auto weights = BarycentricWeights(a, b, c, pointP);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    // Variables to store the interpolated values of U, V, and also 1/w for the
    // current pixel
    float interpolatedU;
    float interpolatedV;
    float interpolated_reciprocal_w;

    // Perform perspective correct interpolation of U/w, V/w values using the
    // barycentric weights and factor of 1/w


    interpolatedU = (auv.u / pointA.w) * alpha + (buv.u / pointB.w) * beta +
                    (cuv.u / pointC.w) * gamma;
    interpolatedV = (auv.v / pointA.w) * alpha + (buv.v / pointB.w) * beta +
                    (cuv.v / pointC.w) * gamma;
    interpolated_reciprocal_w =
        (1 / pointA.w) * alpha + (1 / pointB.w) * beta + (1 / pointC.w) * gamma;

    // Now we can divide back to both interpolated values by 1/w
    interpolatedU /= interpolated_reciprocal_w;
    interpolatedV /= interpolated_reciprocal_w;

    int textureX = abs((int)(interpolatedU * textureWidth));
    int textureY = abs((int)(interpolatedV * textureHeight));

    Pixel(x, y, texture[textureY * textureWidth + textureX]);
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

  // Draw a textured triangle with the flat-top/flat-bottom method. We split the
  // original triangle in two, half flat-bottom and half-flat top
  //
  //            v0 (x0, y0)
  //                                           / \
  //           /   \
  //          /     \
  //         /       \
  //        /         \
  //v1 (x1, y1) ----v3 (x2, y2)
  //       \_           \
  //          \_         \
  //             \_       \
  //                \_     \
  //                   \    \
  //                     \_  \
  //                        \_\
  //                           \
  //                           v2
  //
  void TexturedTriangle(
      int x0, int y0, float z0, float w0, float u0, float v0,
      int x1, int y1, float z1, float w1, float u1, float v1,
      int x2, int y2, float z2, float w2, float u2, float v2,
      const std::unique_ptr<uint32_t[]>& texture
  ) {
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1) {
      intSwap(&y0, &y1);
      intSwap(&x0, &x1);
      floatSwap(&z0, &z1);
      floatSwap(&w0, &w1);
      floatSwap(&u0, &u1);
      floatSwap(&v0, &v1);
    }
    if (y1 > y2) {
      intSwap(&y1, &y2);
      intSwap(&x1, &x2);
      floatSwap(&z1, &z2);
      floatSwap(&w1, &w2);
      floatSwap(&u1, &u2);
      floatSwap(&v1, &v2);
    }
    if (y0 > y1) {
      intSwap(&y0, &y1);
      intSwap(&x0, &x1);
      floatSwap(&z0, &z1);
      floatSwap(&w0, &w1);
      floatSwap(&u0, &u1);
      floatSwap(&v0, &v1);
    }

    auto pointA = Vec<4>{(float)x0, (float)y0, z0, w0};
    auto pointB = Vec<4>{(float)x1, (float)y1, z1, w1};
    auto pointC = Vec<4>{(float)x2, (float)y2, z2, w2};

    UVCoordinates uvA = {u0, v0};
    UVCoordinates uvB = {u1, v1};
    UVCoordinates uvC = {u2, v2};

    //
    // Render the upper part of the triangle (flat-bottom)
    float invSlope1 = 0;
    float invSlope2 = 0;

    if (y1 - y0 > 0) {
      invSlope1 = float(x1 - x0) / (float)(y1 - y0);
    }
    if (y2 - y0 > 0) {
      invSlope2 = float(x2 - x0) / (float)(y2 - y0);
    }

    if (y1 - y0 != 0) {
      for (int y = y0; y <= y1; y++) {
        int xStart = x1 + (y - y1) * invSlope1;
        int xEnd = x0 + (y - y0) * invSlope2;

        if (xEnd < xStart) {
          intSwap(&xStart, &xEnd);
        }

        for (int x = xStart; x < xEnd; x++) {
          Texel(x, y, pointA, pointB, pointC, uvA, uvB, uvC, texture);
        }
      }
    }

    //
    // Render the bottom part of the triangle (flat-top)
    invSlope1 = 0;
    invSlope2 = 0;

    if (y2 - y1 > 0) {
      invSlope1 = float(x2 - x1) / (float)(y2 - y1);
    }
    if (y2 - y0 > 0) {
      invSlope2 = float(x2 - x0) / (float)(y2 - y0);
    }

    if (y2 - y1 != 0) {
      for (int y = y1; y <= y2; y++) {
        int xStart = x1 + (y - y1) * invSlope1;
        int xEnd = x0 + (y - y0) * invSlope2;

        if (xEnd < xStart) {
          intSwap(&xStart, &xEnd);
        }

        for (int x = xStart; x < xEnd; x++) {
          Texel(x, y, pointA, pointB, pointC, uvA, uvB, uvC, texture);
        }
      }
    }
  }

  void Mesh(
      Mesh mesh, bool shouldCull, Camera camera, Light light,
      const std::unique_ptr<uint32_t[]>& texture
  ) {
    std::vector<struct Triangle> trianglesToRender;

    auto mat4x4Scale =
        Matrix4x4::CreateScaleMatrix(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    auto mat4x4Rotation = Matrix4x4::CreateRotationMatrixX(mesh.rotation.x) *
                          Matrix4x4::CreateRotationMatrixY(mesh.rotation.y) *
                          Matrix4x4::CreateRotationMatrixZ(mesh.rotation.z);
    auto mat4x4Translation = Matrix4x4::CreateTranslationMatrix(
        mesh.translation.x,
        mesh.translation.y,
        mesh.translation.z
    );

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

        // order matters
        auto worldMatrix = mat4x4Translation * mat4x4Rotation * mat4x4Scale;
        Vec<4> transformed_vertex4D = worldMatrix * vertex4D;
        Vec<3> transformed_vertex = transformed_vertex4D.ToVec3();

        // Save transformed vertex in the array of transformed vertices
        transformedVertices[j] = transformed_vertex;
      }

      Vec<3> vector_a = transformedVertices[0]; /*   A   */
      Vec<3> vector_b = transformedVertices[1]; /*  / \  */
      Vec<3> vector_c = transformedVertices[2]; /* C---B */

      // Get the vector subtraction of B-A and C-A
      Vec<3> vector_ab = vector_b - vector_a;
      Vec<3> vector_ac = vector_c - vector_a;

      // Compute the face normal (using cross product to find perpendicular)
      Vec<3> normal = vector_ab.CrossProduct(vector_ac);

      if (shouldCull) {
        auto vector_a = transformedVertices[0];
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
        Vec<4> projected_point =
            camera.Project3DPointMatrix(transformedVertices[j]);

        // Scale into the view
        projected_point.x *= (colorBuffer.windowWidth / 2);
        projected_point.y *= (colorBuffer.windowHeight / 2);

        // invert Y axis to account for flipped screen coordinates
        projected_point.y *= -1;

        // Scale and translate the projected points to the middle of the screen
        projected_point.x += (colorBuffer.windowWidth / 2);
        projected_point.y += (colorBuffer.windowHeight / 2);
        projected_point.z = projected_point.z;
        projected_point.w = projected_point.w;

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

      projected_triangle.lightIntensityFactor =
          -normal.Normalize().DotProduct(light.direction);

      projected_triangle.p1_uv = {meshFace.a_uv.u, meshFace.a_uv.v};
      projected_triangle.p2_uv = {meshFace.b_uv.u, meshFace.b_uv.v};
      projected_triangle.p3_uv = {meshFace.c_uv.u, meshFace.c_uv.v};

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

    for (const auto& triangle : trianglesToRender) {
      // Draw unfilled triangle
      //            FilledTriangle(
      //                triangle.p1.x,
      //                triangle.p1.y,  // vertex A
      //                triangle.p2.x,
      //                triangle.p2.y,  // vertex B
      //                triangle.p3.x,
      //                triangle.p3.y,  // vertex C
      //                Colors::Darken(Colors::GREEN,
      //                triangle.lightIntensityFactor)
      //            );
      TexturedTriangle(
          triangle.p1.x,
          triangle.p1.y,
          triangle.p1.z,
          triangle.p1.w,
          triangle.p1_uv.u,
          triangle.p1_uv.v,  // vertex A
          triangle.p2.x,
          triangle.p2.y,
          triangle.p2.z,
          triangle.p2.w,
          triangle.p2_uv.u,
          triangle.p2_uv.v,  // vertex B
          triangle.p3.x,
          triangle.p3.y,
          triangle.p3.z,
          triangle.p3.w,
          triangle.p3_uv.u,
          triangle.p3_uv.v,  // vertex C
          texture
      );
      //            Triangle(
      //                triangle.p1.x,
      //                triangle.p1.y,  // vertex A
      //                triangle.p2.x,
      //                triangle.p2.y,  // vertex B
      //                triangle.p3.x,
      //                triangle.p3.y,  // vertex C
      //                Colors::Darken(Colors::GREEN,
      //                triangle.lightIntensityFactor)
      //            );
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
  Vec<3> BarycentricWeights(Vec<2> a, Vec<2> b, Vec<2> c, Vec<2> p) {
    // Find the vectors between the vertices ABC and point p
    Vec<2> ac = c - a;
    Vec<2> ab = b - a;
    Vec<2> pc = c - p;
    Vec<2> pb = b - p;
    Vec<2> ap = p - a;

    // Area of the full parallelogram (triangle ABC) using cross product
    float area_abc = ac.x * ab.y - ac.y * ab.x;  // |AC x AB|

    // Alpha = area of parallelogram- PBC over the area of the full
    // parallelogram-ABC
    float alpha = (pc.x * pb.y - pc.y * pb.x) / area_abc;

    // Beta = area of parallelogram-APC over the area of the full
    // parallelogram-ABC
    float beta = (ac.x * ap.y - ac.y * ap.x) / area_abc;

    // Gama can be easily found since the barycentric coordinates always add up
    // to 1.0
    float gamma = 1.0f - alpha - beta;

    return Vec<3>{alpha, beta, gamma};
  }

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
