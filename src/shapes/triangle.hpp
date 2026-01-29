

// shapes/triangle.hpp
#pragma once
#include "shape.hpp"
#include <iostream>

// Triangle class derived from Shape
class Triangle : public Shape {
public:
  Vec3 v0, v1, v2;          // Triangle vertices
  Vec2 uv0, uv1, uv2;       // Vertex UVs
  float u = 0.0f, v = 0.0f; // Barycentric coordinates

  // Constructor with positions only
  Triangle(const Vec3 &_v0, const Vec3 &_v1, const Vec3 &_v2, Material &m)
      : Shape(m), v0(_v0), v1(_v1), v2(_v2) {}

  // Constructor with positions + UVs
  Triangle(const Vec3 &_v0, const Vec3 &_v1, const Vec3 &_v2, const Vec2 &_uv0,
           const Vec2 &_uv1, const Vec2 &_uv2, Material &m)
      : Shape(m), v0(_v0), v1(_v1), v2(_v2), uv0(_uv0), uv1(_uv1), uv2(_uv2) {}

  bool intersect(Ray &ray, Hit &hit) {
    Vec3 e0 = v1 - v0;
    Vec3 e1 = v2 - v0;
    Vec3 N = e0.cross(e1);
    Vec3 h_vec = N * 0.5f;

    // ========== PART 1: FINITE-PLANE GUARD (PRT) ==========
    float h = ray.dir.dot(h_vec);

    if (h >= 0 || fabs(h) < 1e-8f) {
      return false;
    }

    // ========== PART 2: SOLVE FOR t ON FINITE PLANE (PRT) ==========
    float fastHit = (v0 - ray.origin).dot(h_vec);
    float t = fastHit / h;

    if (t < 1e-8f) {
      return false;
    }

    // ========== PART 3: GET INTERSECTION POINT ==========
    Vec3 P = ray.origin + ray.dir * t;

    // ========== PART 4: PURE VECTOR ALIGNMENT (PRT) ==========
    Vec3 e0_norm = e0.norm();
    Vec3 e1_norm = e1.norm();

    float u = (P - v0).dot(e0_norm);
    float v = (P - v0).dot(e1_norm);
    float w = 1.0f - u - v;

    // ========== PART 5: BOUNDS CHECK ==========
    if (u < 0 || u > 1 || v < 0 || v > 1 || w < 0 || w > 1) {
      return false;
    }

    // ========== RECORD HIT ==========
    hit.tHit = t;
    hit.point = P;
    hit.normal = N.norm();
    hit.mat = this->mat;
    hit.u = u;
    hit.v = v;
    hit.uv = uv0 * u + uv1 * v + uv2 * w;

    std::cout << "PRT Hit at t=" << t << " u=" << u << " v=" << v << " w=" << w
              << "\n";
    return true;
  }

  ShapeType getShapeType() override { return ShapeType::Triangle; }
};
