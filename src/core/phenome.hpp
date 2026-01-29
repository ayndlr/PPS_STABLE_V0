// core/phenome.hpp
#include "../materials/material.hpp"
#include "math.hpp"
#pragma once

inline bool isEmissive(const Material &m) {
  return (m.emmisive.albedo.x > 0.0f || m.emmisive.albedo.y > 0.0f ||
          m.emmisive.albedo.z > 0.0f);
}

inline bool hasEmissiveDir(const Material &m) {
  // treat near-zero vector as “no direction” (isotropic)
  return m.emmisive.dir.len() > 1e-6f;
}

inline Vec3 ppsWiggleRoughness(const Vec3 &I, const Vec3 &N, float r) {
  return (I + N * r).norm();
}

inline Vec3 ppsScalarRoughness(const Vec3 &I, float r) {
  float s = std::max(0.0f, 1.0f - r); // prevent negative scale
  return (I * s).norm();
}

inline Vec3 ppsReflect(const Vec3 &I, const Vec3 &N, Material &mat) {
  Vec3 I_ = I;
  if (mat.roughness != 0) {
    I_ = ppsScalarRoughness(I, mat.roughness);
    if (mat.wiggleRoughness) {
      I_ = ppsWiggleRoughness(I, N, mat.roughness);
    }
  }
  return (I_ - N * (2.0f * I_.dot(N))).norm();
}

inline Vec3 ppsRefract(const Vec3 &I, const Vec3 &N, Material &mat) {
  // Guard: invalid IOR → reflect
  if (mat.ior <= 1e-6f) {
    return ppsReflect(I, N, mat);
  }

  Vec3 I_ = I.norm();
  Vec3 N_ = N.norm();

  // Roughness first
  if (mat.roughness != 0.0f) {
    I_ = ppsScalarRoughness(I_, mat.roughness);
    if (mat.wiggleRoughness) {
      I_ = ppsWiggleRoughness(I_, N_, mat.roughness);
    }
  }

  // Entry/exit: face-forward normal
  float c = I_.dot(N_);
  bool entering = (c < 0.0f);
  Vec3 Nface = entering ? N_ : N_ * -1;

  // If you only store a single IOR, assume outside=1.0
  float n1 = entering ? 1.0f : mat.ior;
  float n2 = entering ? mat.ior : 1.0f;
  float eta = n1 / n2;

  float cosi = -I_.dot(Nface);
  float k = 1.0f - eta * eta * (1.0f - cosi * cosi);

  if (k < 0.0f) {
    // Total internal reflection
    return ppsReflect(I_, N_, mat);
  }

  Vec3 T = (I_ * eta + Nface * (eta * cosi - std::sqrt(k))).norm();
  return T;
}

inline Vec3 ppsRefractd(const Vec3 &I, const Vec3 &N, Material &mat) {
  Vec3 I_ = I.norm();
  Vec3 N_ = N.norm();

  if (mat.roughness != 0.0f) {
    I_ = ppsScalarRoughness(I_, mat.roughness);
  }

  float c = I_.dot(N_);
  float eta = (c < 0.0f) ? (1.0f / mat.ior) : mat.ior;

  // FORCE: Only refract on entry, always reflect on exit
  if (c > 0.0f) {  // Exiting sphere
    return ppsReflect(I_, N_, mat);
  }

  if (eta * c >= 0.0f) {
    return ppsReflect(I_, N_, mat);
  }

  Vec3 I_parallel = N_ * c;
  Vec3 I_perp = I_ - I_parallel;
  Vec3 refracted = I_perp + I_parallel * eta;
  return refracted.norm();
}
