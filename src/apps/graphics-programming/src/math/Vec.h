#pragma once

#include <array>
#include <cmath>
#include <initializer_list>

template <size_t N>
class Vec {
 public:
  std::array<float, N> elements;

  Vec() { elements.fill(0); }

  Vec(std::initializer_list<float> list) {
    std::copy(list.begin(), list.end(), elements.begin());
  }

  float Length() const {
    float sum = 0;
    for (auto e : elements) {
      sum += e * e;
    }
    return std::sqrt(sum);
  }

  Vec<N> operator+(const Vec<N>& other) const {
    Vec<N> result;
    for (size_t i = 0; i < N; ++i) {
      result.elements[i] = elements[i] + other.elements[i];
    }
    return result;
  }

  Vec<N> operator-(const Vec<N>& other) const {
    Vec<N> result;
    for (size_t i = 0; i < N; ++i) {
      result.elements[i] = elements[i] - other.elements[i];
    }
    return result;
  }

  Vec<N> operator*(float scalar) const {
    Vec<N> result;
    for (size_t i = 0; i < N; ++i) {
      result.elements[i] = elements[i] * scalar;
    }
    return result;
  }

  Vec<N> operator/(float scalar) const {
    Vec<N> result;
    for (size_t i = 0; i < N; ++i) {
      result.elements[i] = elements[i] / scalar;
    }
    return result;
  }

  float& operator[](size_t index) { return elements[index]; }

  const float& operator[](size_t index) const { return elements[index]; }
};

template <>
class Vec<2> {
 public:
  union {
    struct {
      float x, y;
    };
    std::array<float, 2> elements{};
  };

  explicit Vec(float x = 0, float y = 0) : x(x), y(y) {}
};

template <>
class Vec<3> {
 public:
  union {
    struct {
      float x, y, z;
    };
    std::array<float, 3> elements{};
  };

  explicit Vec(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

  Vec<3> RotateX(float angle) const {
    float rad = angle * M_PI / 180;
    float cosa = cos(rad);
    float sina = sin(rad);
    return Vec<3>(x, y * cosa - z * sina, y * sina + z * cosa);
  }

  Vec<3> RotateY(float angle) const {
    float rad = angle * M_PI / 180;
    float cosa = cos(rad);
    float sina = sin(rad);
    return Vec<3>(x * cosa - z * sina, y, x * sina + z * cosa);
  }

  Vec<3> RotateZ(float angle) const {
    float rad = angle * M_PI / 180;
    float cosa = cos(rad);
    float sina = sin(rad);
    return Vec<3>(x * cosa - y * sina, x * sina + y * cosa, z);
  }

  Vec<3> operator-(const Vec<3>& other) const {
    return Vec<3>(x - other.x, y - other.y, z - other.z);
  }

  Vec<3> CrossProduct(const Vec<3>& other) const {
    return Vec<3>(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
  }

  Vec<3> Normalize() const {
    float length = Length();
    return Vec<3>(x / length, y / length, z / length);
  }

  float DotProduct(Vec<3> other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  float Length() const {
    return std::sqrt(x * x + y * y + z * z);
  }
};
