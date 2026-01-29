// core/aabb.hpp
#pragma once
#include "math.hpp"

struct AABB {
  Vec3 min, max;

  AABB() : min(Vec3()), max(Vec3()) {}
  AABB(const Vec3 &minV, const Vec3 &maxV) : min(minV), max(maxV) {}

  // Expand box to include another point
  void expand(const Vec3 &p) {
    min.x = std::min(min.x, p.x);
    min.y = std::min(min.y, p.y);
    min.z = std::min(min.z, p.z);

    max.x = std::max(max.x, p.x);
    max.y = std::max(max.y, p.y);
    max.z = std::max(max.z, p.z);
  }

  // Expand to include another AABB
  void expand(const AABB &other) {
    expand(other.min);
    expand(other.max);
  }
};
