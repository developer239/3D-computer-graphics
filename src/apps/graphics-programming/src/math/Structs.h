#pragma once

struct UVCoordinates {
  float u;
  float v;

  UVCoordinates operator*(float scalar) const {
    UVCoordinates result;
    result.u = u * scalar;
    result.v = v * scalar;
    return result;
  }

  UVCoordinates operator+(const UVCoordinates& other) const {
    UVCoordinates result;
    result.u = u + other.u;
    result.v = v + other.v;
    return result;
  }
};
