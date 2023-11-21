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

  static Matrix4x4 CreatePerspectiveMatrix(
      float flov, float aspect, float znear, float zfar
  ) {
    //| 1/aspect*tan(flov/2) 0 0 0 |
    //| 0 1/tan(flov/2) 0 0 |
    //| 0 0 zfar/(zfar-znear) znear*zfar/(znear-zfar) |
    //| 0 0 1 0 |
    float tanHalfFlov = tan(flov / 2);
    return Matrix4x4({
        {1 / (aspect * tanHalfFlov), 0, 0, 0},
        {0, 1 / tanHalfFlov, 0, 0},
        {0, 0, zfar / (zfar - znear), znear * zfar / (znear - zfar)},
        {0, 0, 1, 0},
    });
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

  Matrix4x4 static CreateRotationMatrixZ(float angle) {
    //| cos -sin 0 0 |
    //| sin cos  0 0 |
    //| 0   0    1 0 |
    //| 0   0    0 1 |
    float rad = angle * M_PI / 180;
    float cosa = cos(rad);
    float sina = sin(rad);
    return Matrix4x4(
        {{cosa, -sina, 0, 0}, {sina, cosa, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}
    );
  }

  Matrix4x4 static CreateRotationMatrixX(float angle) {
    //| 1 0    0   0 |
    //| 0 cos -sin 0 |
    //| 0 sin cos  0 |
    //| 0 0   0    1 |
    float rad = angle * M_PI / 180;
    float cosa = cos(rad);
    float sina = sin(rad);
    return Matrix4x4(
        {{1, 0, 0, 0}, {0, cosa, -sina, 0}, {0, sina, cosa, 0}, {0, 0, 0, 1}}
    );
  }

  Matrix4x4 static CreateRotationMatrixY(float angle) {
    //| cos  0 sin 0 |
    //| 0    1 0   0 |
    //| -sin 0 cos 0 |
    //| 0    0 0   1 |
    float rad = angle * M_PI / 180;
    float cosa = cos(rad);
    float sina = sin(rad);
    return Matrix4x4(
        {{cosa, 0, sina, 0}, {0, 1, 0, 0}, {-sina, 0, cosa, 0}, {0, 0, 0, 1}}
    );
  }

  std::array<std::array<float, 4>, 4> data{};

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
