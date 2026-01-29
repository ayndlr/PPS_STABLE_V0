// core//ray.hpp
#pragma once
#include "math.hpp"

struct Ray {
    Vec3 origin;
    Vec3 dir;
    float energy = 1.0f; // PPS energy tracking
    int depth = 0;       // recursion depth

    Ray() : origin(0,0,0), dir(0,0,0), energy(1.0f), depth(0) {}
    Ray(const Vec3 &o, const Vec3 &d, float e=1.0f, int dep=0) 
        : origin(o), dir(d.norm()), energy(e), depth(dep) {}
    
    Vec3 pointAt(float t) const { return origin + dir * t; }
};

class Camera {
public:
  Vec3 pos;
  Vec3 forward;
  Vec3 right;
  Vec3 up;

  float fov; // vertical fov in degrees
  float aspect;

  Camera(const Vec3 &pos, const Vec3 &lookAt, float fovDeg, float aspectRatio)
      : pos(pos), fov(fovDeg), aspect(aspectRatio) {
    forward = (lookAt - pos).norm();
    Vec3 worldUp(0, 1, 0);
    right = forward.cross(worldUp).norm();
    up = right.cross(forward).norm();
  }

  // core/ray.hpp

  Ray getRay(float x, float y, int W, int H) const { // ADD W and H parameters
    float u = x / W; // Normalize pixel x to [0, 1]
    float v = y / H; // Normalize pixel y to [0, 1]

    float alpha = tan(fov * 0.5 * M_PI / 180.0);
    Vec3 rayDir = forward + right * (2 * u - 1) * alpha * aspect +
                  up * (1 - 2 * v) * alpha;
    return Ray(pos, rayDir.norm());
  }
};

struct Light {
  Vec3 pos;
  Vec3 color;
  Vec3 dir; // if zero vector, treat as point light
  float energy;

  Light() : pos(0, 0, 0), color(1, 1, 1), dir(0, 0, 0) {}
  Light(const Vec3 &pos, const Vec3 &col, const Vec3 &dir = Vec3(0, 0, 0))
      : pos(pos), color(col), dir(dir) {}
};
