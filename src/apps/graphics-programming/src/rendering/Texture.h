#pragma once

#include <SDL.h>
#include <cstring>
#include <memory>

#ifdef __cplusplus
extern "C" {
#endif
#include "upng.h"
#ifdef __cplusplus
}
#endif

class Texture {
 public:
  int width;
  int height;
  std::unique_ptr<uint32_t[]> data;

  Texture() : width(0), height(0), data(nullptr) {}

  Texture(const std::string& pathToFile) {
    upng_t* texture = upng_new_from_file(pathToFile.c_str());
    if (texture == nullptr) {
      printf("Error: could not load texture %s\n", pathToFile.c_str());
      return;
    }

    upng_decode(texture);
    if (upng_get_error(texture) != UPNG_EOK) {
      printf("Error: could not decode texture %s\n", pathToFile.c_str());
      return;
    }

    width = upng_get_width(texture);
    height = upng_get_height(texture);

    size_t bufferSize = upng_get_size(texture);
    data = std::make_unique<uint32_t[]>(bufferSize / sizeof(uint32_t));
    std::memcpy(data.get(), upng_get_buffer(texture), bufferSize);

    upng_free(texture);
  }

  // Copy constructor
  Texture(const Texture& other)
      : width(other.width),
        height(other.height),
        data(new uint32_t[other.width * other.height]) {
    std::memcpy(
        data.get(),
        other.data.get(),
        width * height * sizeof(uint32_t)
    );
  }

  // Move constructor
  Texture(Texture&& other) noexcept
      : width(other.width), height(other.height), data(std::move(other.data)) {
    other.width = 0;
    other.height = 0;
  }

  // Copy assignment operator
  Texture& operator=(const Texture& other) {
    if (this != &other) {
      width = other.width;
      height = other.height;
      data = std::make_unique<uint32_t[]>(width * height);
      std::memcpy(
          data.get(),
          other.data.get(),
          width * height * sizeof(uint32_t)
      );
    }
    return *this;
  }

  // Move assignment operator
  Texture& operator=(Texture&& other) noexcept {
    if (this != &other) {
      width = other.width;
      height = other.height;
      data = std::move(other.data);
      other.width = 0;
      other.height = 0;
    }
    return *this;
  }

  // Destructor is not needed as unique_ptr will handle memory deallocation
};
