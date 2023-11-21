#pragma once

#include <SDL.h>

#include "vector"

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
  uint32_t* data;

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

    width = 64;
    height = 64;

    size_t bufferSize = upng_get_size(texture);
    data = new uint32_t[bufferSize / sizeof(uint32_t)];
    std::memcpy(data, upng_get_buffer(texture), bufferSize);
    upng_free(texture);
  }
};
