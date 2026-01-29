// shapes/sphere.hpp
#pragma once
#include "shapes.hpp"
#include <iostream>

class Sphere : public Shape {
public:
  Vec3 position;
  float radius;

  // Constructor
  Sphere(const Vec3 &_position, float _radius, Material &m)
      : Shape(m), position(_position), radius(_radius) {}

  bool intersect(Ray &ray, Hit &hit) override {
    // ========== SPHERE RAY INTERSECTION ==========
    // Ray: P(t) = o + t*d
    // Sphere: |P - c|^2 = r^2
    // Substitute: |o + t*d - c|^2 = r^2
    
    Vec3 oc = ray.origin - position;  // Vector from sphere center to ray origin
    
    // Quadratic equation: (d·d)t^2 + 2(oc·d)t + (oc·oc - r^2) = 0
    float a = ray.dir.dot(ray.dir);
    float b = 2.0f * oc.dot(ray.dir);
    float c = oc.dot(oc) - radius * radius;
    
    // Discriminant
    float discriminant = b * b - 4.0f * a * c;
    
    // No intersection
    if (discriminant < 0) {
      return false;
    }
    
    // Two solutions
    float sqrt_disc = std::sqrt(discriminant);
    float t1 = (-b - sqrt_disc) / (2.0f * a);
    float t2 = (-b + sqrt_disc) / (2.0f * a);
    
    // Take closest positive t
    float t = -1.0f;
    if (t1 > 1e-8f) {
      t = t1;
    } else if (t2 > 1e-8f) {
      t = t2;
    } else {
      return false;
    }
    
    // ========== RECORD HIT ==========
    Vec3 P = ray.origin + ray.dir * t;
    Vec3 normal = (P - position).norm();  // Normal points outward from sphere
    
    hit.tHit = t;
    hit.point = P;
    hit.normal = normal;
    hit.mat = this->mat;
    hit.u = 0.0f;  // Placeholder (would need UV mapping)
    hit.v = 0.0f;
    hit.uv = Vec2(0, 0);
    
    
    return true;
  }

  ShapeType getShapeType() override { return ShapeType::Sphere; }
};
