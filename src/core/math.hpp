// core/math.hpp
#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>

struct Vec2 {
  float x, y;

  // Constructors
  Vec2() : x(0), y(0) {}
  Vec2(float X, float Y) : x(X), y(Y) {}

  // Operators
  Vec2 operator+(const Vec2 &v) const { return Vec2(x + v.x, y + v.y); }
  Vec2 operator-(const Vec2 &v) const { return Vec2(x - v.x, y - v.y); }
  Vec2 operator*(float s) const { return Vec2(x * s, y * s); }
  Vec2 operator/(float s) const { return Vec2(x / s, y / s); }
  Vec2 operator*(const Vec2 &v) const { return Vec2(x * v.x, y * v.y); }

  Vec2 &operator+=(const Vec2 &v) {
    x += v.x;
    y += v.y;
    return *this;
  }
  Vec2 &operator*=(const Vec2 &v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }
  Vec2 &operator*=(float s) {
    x *= s;
    y *= s;
    return *this;
  }

  float operator[](int i) const { return i == 0 ? x : y; }
  float &operator[](int i) { return i == 0 ? x : y; }

  bool isEmpty() const { return x == 0.0f && y == 0.0f; }

  // Vector operations
  float dot(const Vec2 &v) const { return x * v.x + y * v.y; }

  float len() const { return std::sqrt(x * x + y * y); }
  Vec2 norm() const {
    float l = len();
    return Vec2(x / l, y / l);
  }

  Vec2 clamp0() const {
    return Vec2(std::max(0.0f, std::min(1.0f, x)),
                std::max(0.0f, std::min(1.0f, y)));
  }

  bool isNan() const { return std::isnan(x) || std::isnan(y); }
  bool isInf() const { return std::isinf(x) || std::isinf(y); }

  void printComp() const {
    std::cout << "\tVector2 Component: "
              << "X:" << x << " Y:" << y << "\n";
  }
};

struct Vec3 {
  float x, y, z;

  // Constructors
  Vec3() : x(0), y(0), z(0) {}
  Vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

  // Operators
  Vec3 operator+(const Vec3 &v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
  }
  Vec3 operator-(const Vec3 &v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
  }
  Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }

  Vec3 operator*(const Vec3 &b) const { return {x * b.x, y * b.y, z * b.z}; }

  Vec3 operator/(float s) const { return Vec3(x / s, y / s, z / s); }

  Vec3 &operator+=(const Vec3 &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vec3 &operator*=(const Vec3 &v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  Vec3 &operator*=(float s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }

  float operator[](int i) const {
    if (i == 0)
      return x;
    if (i == 1)
      return y;
    return z;
  }

  float &operator[](int i) {
    if (i == 0)
      return x;
    if (i == 1)
      return y;
    return z;
  }

  bool isEmpty() const {
    if (x == 0.0 && y == 0.0 && z == 0.0) {
      return true;
    }
    return false;
  }

  // Vector operations
  float dot(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }
  Vec3 cross(const Vec3 &v) const {
    return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
  }

  float len() const { return std::sqrt(x * x + y * y + z * z); }

  Vec3 norm() const {
    float l = len();
    return Vec3(x / l, y / l, z / l);
  }

  // Clamp each component between 0 and 1
  Vec3 clamp0() const {
    return Vec3(std::max(0.0f, std::min(1.0f, x)),
                std::max(0.0f, std::min(1.0f, y)),
                std::max(0.0f, std::min(1.0f, z)));
  }

  bool isNan() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }

  bool isInf() const { return std::isinf(x) || std::isinf(y) || std::isinf(z); }

  void printComp() {
    std::cout << "\t"
              << "Vector Component: "
              << " X:" << x << " Y: " << y << " Z: " << z << "\n";
  }
};
