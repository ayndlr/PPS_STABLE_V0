// shapes/plane.hpp
#pragma once
#include "shape.hpp"
#include <iostream>

class Plane : public Shape {
public:
  Vec3 position;
  Vec3 normal;
  float size;  // Half-size of the square (0.5 for 1x1 squares)

  // Constructor
  Plane(const Vec3 &_position, const Vec3 &_normal, Material &m, float _size = 0.5f)
      : Shape(m), position(_position), normal(_normal.norm()), size(_size) {}

  bool intersect(Ray &ray, Hit &hit) override {
    float denom = ray.dir.dot(normal);
    if (fabs(denom) < 1e-8f) {
      return false;
    }
    
    float t = (position - ray.origin).dot(normal) / denom;
    
    if (t < 1e-8f) {
      return false;
    }
    
    Vec3 P = ray.origin + ray.dir * t;
    
    // Bounds check: only accept hits within the square
    Vec3 delta = P - position;
    
    // For a horizontal plane (Y = const), check X and Z bounds
    if (fabs(delta.x) > size || fabs(delta.z) > size) {
      return false;  // Outside the square
    }
    
    // Flip normal if needed
    Vec3 hitNormal = normal;
    if (denom > 0) {
      hitNormal = hitNormal * -1.0f;
    }
    
    hit.tHit = t;
    hit.point = P;
    hit.normal = hitNormal;
    hit.mat = this->mat;
    
    return true;
  }

  ShapeType getShapeType() override { return ShapeType::Plane; }
};

/*// shapes/plane.hpp
#pragma once
#include "shape.hpp"
#include <iostream>

class Plane : public Shape {
public:
  Vec3 position;
  Vec3 normal;

  // Constructor
  Plane(const Vec3 &_position, const Vec3 &_normal, Material &m)
      : Shape(m), position(_position), normal(_normal.norm()) {}

  bool intersect(Ray &ray, Hit &hit) override {
    // ========== PLANE RAY INTERSECTION ==========
    // Plane equation: (P - position) · normal = 0
    // Ray: P(t) = o + t*d
    // Substitute: (o + t*d - position) · normal = 0
    
    float denom = ray.dir.dot(normal);
    // Ray is parallel to plane
    if (fabs(denom) < 1e-8f) {
      return false;
    }
    
    // Solve for t
    float t = (position - ray.origin).dot(normal) / denom;
    
    if (t < 1e-8f) {
      return false;
    }
    
    // ========== RECORD HIT ==========
    Vec3 P = ray.origin + ray.dir * t;
    
    // Flip normal if ray is hitting from behind
    Vec3 hitNormal = normal;
    if (denom > 0) {
      hitNormal = hitNormal * -1.0f;
    }
    
    hit.tHit = t;
    hit.point = P;
    hit.normal = hitNormal;
    hit.mat = this->mat;
    hit.u = 0.0f;
    hit.v = 0.0f;
    hit.uv = Vec2(0, 0);
    
    return true;
  }

  ShapeType getShapeType() override { return ShapeType::Plane; }
};
*/