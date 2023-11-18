#include <array>
#include <cmath>
#include <initializer_list>

class Matrix4x4 {
 public:
  Matrix4x4 static CreateIdentityMatrix() {
    //| 1 0 0 0 |
    //| 0 1 0 0 |
    //| 0 0 1 0 |
    //| 0 0 0 1 |
    return Matrix4x4({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
  }

  Matrix4x4 static CreateScaleMatrix(float x, float y, float z) {
    //| x 0 0 0 |
    //| 0 y 0 0 |
    //| 0 0 z 0 |
    //| 0 0 0 1 |
    return Matrix4x4({{x, 0, 0, 0}, {0, y, 0, 0}, {0, 0, z, 0}, {0, 0, 0, 1}});
  }

  Matrix4x4 static CreateTranslationMatrix(float x, float y, float z) {
    //| 1 0 0 x |
    //| 0 1 0 y |
    //| 0 0 1 z |
    //| 0 0 0 1 |
    return Matrix4x4({{1, 0, 0, x}, {0, 1, 0, y}, {0, 0, 1, z}, {0, 0, 0, 1}});
  }

  Matrix4x4 static CreateRotationMatrix(float x, float y, float z) {
    // | cosY * cosZ, cosZ * sinX * sinY - cosX * sinZ, cosX * cosZ * sinY +
    // sinX * sinZ, 0 |
    // | cosY * sinZ, cosX * cosZ + sinX * sinY * sinZ, cosX *
    // sinY * sinZ - cosZ * sinX, 0 | | -sinY, cosY * sinX, cosX * cosY, 0 |
    // | 0, 0, 0, 1 |
    float radX = x * M_PI / 180;
    float radY = y * M_PI / 180;
    float radZ = z * M_PI / 180;

    float cosX = cos(radX);
    float cosY = cos(radY);
    float cosZ = cos(radZ);
    float sinX = sin(radX);
    float sinY = sin(radY);
    float sinZ = sin(radZ);

    return Matrix4x4(
        {{cosY * cosZ,
          cosZ * sinX * sinY - cosX * sinZ,
          cosX * cosZ * sinY + sinX * sinZ,
          0},
         {cosY * sinZ,
          cosX * cosZ + sinX * sinY * sinZ,
          cosX * sinY * sinZ - cosZ * sinX,
          0},
         {-sinY, cosY * sinX, cosX * cosY, 0},
         {0, 0, 0, 1}}
    );
  }

  std::array<std::array<float, 4>, 4> data;

  Matrix4x4(std::initializer_list<std::array<float, 4>> list) {
    int i = 0;
    for (auto& row : list) {
      data[i++] = row;
    }
  }

  Vec<4> operator*(const Vec<4>& vec) const {
    return Vec<4>{
        data[0][0] * vec.x + data[0][1] * vec.y + data[0][2] * vec.z +
            data[0][3] * vec.w,
        data[1][0] * vec.x + data[1][1] * vec.y + data[1][2] * vec.z +
            data[1][3] * vec.w,
        data[2][0] * vec.x + data[2][1] * vec.y + data[2][2] * vec.z +
            data[2][3] * vec.w,
        data[3][0] * vec.x + data[3][1] * vec.y + data[3][2] * vec.z +
            data[3][3] * vec.w,
    };
  }

  Matrix4x4 operator*(const Matrix4x4& other) const {
    std::array<std::array<float, 4>, 4> result = {};
    for (int i = 0; i < 4; i++) {
      for (int k = 0; k < 4; k++) {
        float sum = 0;
        for (int j = 0; j < 4; j++) {
          sum += data[i][j] * other.data[j][k];
        }
        result[i][k] = sum;
      }
    }

    return Matrix4x4({
        result[0],
        result[1],
        result[2],
        result[3],
    });
  }
};
