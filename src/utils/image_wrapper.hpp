// utils/image_wrapper.hpp
#pragma once
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../core/math.hpp"
#include "stb_image_write.h"
#include <vector>

inline void saveImage(const char *filename, int width, int height,
                      const std::vector<Vec3> &framebuffer) {
  std::vector<unsigned char> pixels(width * height * 3);
  for (int i = 0; i < width * height; i++) {
    pixels[i * 3 + 0] =
        static_cast<unsigned char>(std::min(1.0f, framebuffer[i].x) * 255.0);
    pixels[i * 3 + 1] =
        static_cast<unsigned char>(std::min(1.0f, framebuffer[i].y) * 255.0);
    pixels[i * 3 + 2] =
        static_cast<unsigned char>(std::min(1.0f, framebuffer[i].z) * 255.0);
  }
  stbi_write_png(filename, width, height, 3, pixels.data(), width * 3);
  std::cout << "Output rendered to " << filename << "\n";
}

// For hdr using stb
