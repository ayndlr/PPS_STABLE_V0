// shapes/shape.hpp
#pragma once
#include "../core/math.hpp"
#include "../core/ray.hpp"
#include "../materials/material.hpp"

struct Hit {
  Vec3 point;         // Intersection point in 3D
  Vec3 normal;        // Surface normal at hit
  float u = 0.0f;     // Barycentric coordinate
  float v = 0.0f;     // Barycentric coordinate
  float tHit=0.0f; // Ray parameter
  Material mat;

  Vec2 uv; // Interpolated texture coordinate
};

enum class ShapeType { Triangle, Quad, Plane, Sphere };

class Shape {
public:
  Material &mat;
  Hit hit;
  Shape(Material &m) : mat(m) {  }
  ~Shape() {}

  virtual bool intersect(Ray &r, Hit &hit) = 0;

  virtual void normalShade() { hit.mat.albedo = hit.normal; }
  virtual ShapeType getShapeType() = 0;
};
