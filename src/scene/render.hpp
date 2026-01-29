// scene/render.hpp
#pragma once
#include "../core/phenome.hpp"
#include "scene.hpp"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

class PPS {
private:
  std::chrono::high_resolution_clock::time_point pStart;
  int lastPercent = -1;

public:
  int W, H;
  std::vector<Vec3> framebuffer;

  PPS(int Width, int Height) : W(Width), H(Height) {}
  ~PPS() {}

  inline Vec3 emitContribution(const Hit &hit, const Ray &ray) {
    const Material &mat = hit.mat;

    if (!isEmissive(mat))
      return Vec3(0, 0, 0);

    Vec3 E = mat.emmisive.albedo;

    if (!hasEmissiveDir(mat)) {
      return E;
    } else {
      Vec3 edir = mat.emmisive.dir.norm();
      float NdotE = std::max(0.0f, hit.normal.dot(edir));
      return E * NdotE;
    }
  }

  inline Vec3 traceLight(const Ray &ray, const Light &light, Scene &scene,
                         const Hit &hit) {
    Vec3 Ldir;
    float falloff = 1.0f;
    Ray sRay;
    Hit sHit;
    const float tMinShadow = 1e-4f;

    if (light.dir.isEmpty()) {
      Vec3 L = (light.pos - hit.point);
      float dist = L.len();
      Ldir = L / std::max(dist, 1e-6f);
      falloff = 1.0f / std::max(1e-6f, dist * dist);

      sRay.origin = hit.point + Ldir * 2e-4f;
      sRay.dir = Ldir;

      if (scene.intersectScene(sRay, sHit) && sHit.tHit > tMinShadow &&
          sHit.tHit < (dist - 1e-4f))
        return Vec3(0, 0, 0);
    } else {
      Ldir = (light.dir * -1.0f).norm();
      sRay.origin = hit.point + Ldir * 2e-4f;
      sRay.dir = Ldir;

      if (scene.intersectScene(sRay, sHit) && sHit.tHit > tMinShadow)
        return Vec3(0, 0, 0);
    }

    float NdotL = std::max(0.0f, hit.normal.dot(Ldir));
    return hit.mat.albedo * light.color * (light.energy * falloff * NdotL);
  }

  // Path Tracer ~ PPS
  inline Vec3 tracePath(Ray &ray, Scene &scene,
                        const std::vector<Light> &lights, int depth,
                        int maxDepth) {
    if (depth >= maxDepth)
      return Vec3(0, 0, 0);

    Hit hit;
    if (!scene.intersectScene(ray, hit)) {
      return Vec3(0.2f, 0.6f, 0.95f); // background
    }

    Vec3 col(0, 0, 0);

    // 1) Emissive contribution
    col += emitContribution(hit, ray);

    // 2) Direct lighting
    for (const auto &light : lights) {
      col += traceLight(ray, light, scene, hit);
    }

    // 3) Determine next direction
    // For transmissive materials, attempt refraction
    // ppsRefract handles entry/exit/TIR internally
    Vec3 nextDir = hit.mat.transmissive ? ppsRefract(ray.dir, hit.normal, hit.mat)
                                        : ppsReflect(ray.dir, hit.normal, hit.mat);

    Ray nextRay;
    nextRay.origin = hit.point + nextDir * 2e-4f;
    nextRay.dir = nextDir;

    // 4) Attenuation per bounce
    float absorption = std::max(0.0f, hit.mat.absorption);
    float thickness = std::max(0.0f, hit.mat.thickness);
    
    // Only apply attenuation if material absorbs
    float atten = 1.0f;
    if (absorption > 0.0f || thickness > 0.0f) {
      atten = (.85 - std::min(0.9f, absorption * 0.2f)) *
              (.85 - std::min(0.9f, thickness * 0.3f));
    }

    // 5) Recursive bounce
    Vec3 bounce = tracePath(nextRay, scene, lights, depth + 1, maxDepth);
    col += (hit.mat.albedo * atten) * bounce;
    return col;
  }

  void renderScene(Camera &cam, int maxDepth, Scene &scene) {
    std::cout << "Starting PPS Renderer\n";
    std::cout << "Render Resolution ~ Image Res WxH " << W << "x" << H << "\n";
    std::cout << "Depth: " << maxDepth << "\n";
    std::cout << "Shooting " << W * H * maxDepth << " Rays into Scene\n";
    framebuffer.resize(W * H, Vec3(0, 0, 0));
    profileStart();

    int total = W * H;
    int pixelI = 0;

    const auto &lights = scene.getLights();

    for (int y = 0; y < H; ++y) {
      for (int x = 0; x < W; ++x, ++pixelI) {
        showProgress(pixelI, total);
        Ray ray = cam.getRay(x, y, W, H);
        Vec3 col = tracePath(ray, scene, lights, 0, maxDepth);
        framebuffer[y * W + x] = col.clamp0();
      }
    }

    profileStop();
  }

  void profileStart() {
    pStart = std::chrono::high_resolution_clock::now();
    lastPercent = -1;
  }

  std::string formatTime(double seconds) {
    int s = (int)seconds;
    int h = s / 3600;
    s %= 3600;
    int m = s / 60;
    s %= 60;
    char buff[32];
    sprintf(buff, "%02d:%02d:%02d", h, m, s);
    return std::string(buff);
  }

  void profileStop() {
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - pStart).count();
    std::cout << "\nPPS finished in " << formatTime(elapsed) << "\n";
  }

  void showProgress(int current, int total) {
    if (total <= 0)
      return;

    int percent = (100 * current) / total;
    if (percent == lastPercent)
      return;
    lastPercent = percent;

    auto now = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(now - pStart).count();
    double eta = percent > 0 ? elapsed * (100.0 - percent) / percent : 0.0;

    const int barWidth = 30;
    int filled = (percent * barWidth) / 100;
    std::string bar = "[";
    for (int i = 0; i < barWidth; i++) {
      if (i < filled)
        bar += "\033[48;2;0;150;255m \033[0m";
      else
        bar += "\033[48;2;50;50;50m \033[0m";
    }
    bar += "]";
    std::cout << "\r" << bar << " " << percent
              << "%  | ETA: " << formatTime(eta) << "   " << std::flush;
  }
};
