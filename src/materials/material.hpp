// materials/material.hpp
#pragma once
#include "../core/math.hpp"

struct Emmisive {
  // Emmisive light color
  Vec3 albedo{0,0,0};

  // Simulate a emmisive light dir e.g touch light
  Vec3 dir;
};

struct Material {
  // PPS Roughness Values
  // A roughness computed by I*roughness where I is incoming ray
  float roughness = 1.1;
  // A normal base roughness computed by I+(N*rougness);
  float wiggleRoughness = false;

  // PPS SSS system no bxdf
  // SSS intensity
  float sss;
  // SSS layer e.g 2 SSS, 3 Simulate and can be used for human skin sss
  float sssLayer;

  // PPS Transmissive system tells if a object is transmissive to allow ray pass
  // through it
  bool transmissive = false;

  // PPS IOR system influencing how I bend inside a medium do notnuse Fr or
  // Snells
  float ior = 0;

  // PPS Light Attenuation System
  // Simulate material objection
  float absorption = .0;
  // Simulate surface and material thickness
  float thickness = .0;

  // PPS surface,material,and object color
  Vec3 albedo;

  // PPS Emmisive System
  // Describe how emmisve surface,material as simulated
  // It use object defualt position
  Emmisive emmisive;
};
