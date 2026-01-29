// shapes/quad.hpp
#pragma once
#include "shape.hpp"
#include <iostream>

// Quadrilateral class with Projective Ray Tracing (PRT)
class Quad : public Shape {
public:
  Vec3 v0, v1, v2, v3;      // Quad vertices (v0: bottom-left, v1: bottom-right, v2: top-right, v3: top-left)
  Vec2 uv0, uv1, uv2, uv3;  // Vertex UVs

  // Constructor with positions only
  Quad(const Vec3 &_v0, const Vec3 &_v1, const Vec3 &_v2, const Vec3 &_v3, Material &m)
      : Shape(m), v0(_v0), v1(_v1), v2(_v2), v3(_v3) {}

  // Constructor with positions + UVs
  Quad(const Vec3 &_v0, const Vec3 &_v1, const Vec3 &_v2, const Vec3 &_v3,
       const Vec2 &_uv0, const Vec2 &_uv1, const Vec2 &_uv2, const Vec2 &_uv3, Material &m)
      : Shape(m), v0(_v0), v1(_v1), v2(_v2), v3(_v3), 
        uv0(_uv0), uv1(_uv1), uv2(_uv2), uv3(_uv3) {}

  bool intersect(Ray &ray, Hit &hit) {
    // ========== DECOMPOSE QUAD INTO TWO TRIANGLES ==========
    // Triangle 1: v0, v1, v3
    // Triangle 2: v1, v2, v3

    // Try Triangle 1 first
    if (intersectTriangle1(ray, hit)) {
      return true;
    }

    // Try Triangle 2
    if (intersectTriangle2(ray, hit)) {
      return true;
    }

    return false;
  }

private:
  bool intersectTriangle1(const Ray &ray, Hit &hit) {
    Vec3 e0 = v1 - v0;
    Vec3 e1 = v3 - v0;
    Vec3 N = e0.cross(e1);
    Vec3 h_vec = N * 0.5f;

    float h = ray.dir.dot(h_vec);
    if (h >= 0 || fabs(h) < 1e-8f) return false;

    float fastHit = (v0 - ray.origin).dot(h_vec);
    float t = fastHit / h;
    if (t < 1e-8f) return false;

    Vec3 P = ray.origin + ray.dir * t;

    Vec3 e0_norm = e0.norm();
    Vec3 e1_norm = e1.norm();
    
    float u = P.dot(e0_norm);
    float v = P.dot(e1_norm);
    float w = 1.0f - u - v;

    if (u < 0 || u > 1 || v < 0 || v > 1 || w < 0 || w > 1) {
      return false;
    }

    hit.tHit = t;
    hit.point = P;
    hit.normal = N.norm();
    hit.mat = this->mat;
    hit.u = u;
    hit.v = v;
    hit.uv = uv0 * w + uv1 * u + uv3 * v;

    std::cout << "PRT Quad (Tri1) Hit at t=" << t << " u=" << u << " v=" << v << "\n";
    return true;
  }

  bool intersectTriangle2(const Ray &ray, Hit &hit) {
    Vec3 e0 = v2 - v1;
    Vec3 e1 = v3 - v1;
    Vec3 N = e0.cross(e1);
    Vec3 h_vec = N * 0.5f;

    float h = ray.dir.dot(h_vec);
    if (h >= 0 || fabs(h) < 1e-8f) return false;

    float fastHit = (v1 - ray.origin).dot(h_vec);
    float t = fastHit / h;
    if (t < 1e-8f) return false;

    Vec3 P = ray.origin + ray.dir * t;

    Vec3 e0_norm = e0.norm();
    Vec3 e1_norm = e1.norm();
    
    float u = P.dot(e0_norm);
    float v = P.dot(e1_norm);
    float w = 1.0f - u - v;

    if (u < 0 || u > 1 || v < 0 || v > 1 || w < 0 || w > 1) {
      return false;
    }

    hit.tHit = t;
    hit.point = P;
    hit.normal = N.norm();
    hit.mat = this->mat;
    hit.u = u;
    hit.v = v;
    hit.uv = uv1 * w + uv2 * u + uv3 * v;

    std::cout << "PRT Quad (Tri2) Hit at t=" << t << " u=" << u << " v=" << v << "\n";
    return true;
  }

public:
  ShapeType getShapeType() override { return ShapeType::Quad; }
};
