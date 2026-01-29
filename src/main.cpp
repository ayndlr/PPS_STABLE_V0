#include "scene/render.hpp"
#include "utils/image_wrapper.hpp"
#include <string>

using namespace std;

int main() {
  int k = 1;
  int W = (int)1024 * k, H = (int)576 * k, maxDepth = 20;
  // Initialize PPS
  PPS render(W, H);

  // Setting Up Camera
  Vec3 origin = {0, 1.5, 4.5};
  Vec3 dir = {0, 0.5, -1};
  float fov = 45;
  const float &aspect = (float)W / H;
  Camera cam(origin, dir, fov, aspect);

  // Preparing Scene
  Scene scene;

  // Checkerboard floor materials
  Material checkWhite;
  checkWhite.albedo = {0.95, 0.95, 0.95};
  checkWhite.roughness = 0.05;
  checkWhite.transmissive = false;
  checkWhite.ior = 1.0;
  checkWhite.absorption = 0.0;
  checkWhite.thickness = 0.0;

  Material checkDark;
  checkDark.albedo = {0.4, 0.4, 0.4};
  checkDark.roughness = 0.05;
  checkDark.transmissive = false;
  checkDark.ior = 1.0;
  checkDark.absorption = 0.0;
  checkDark.thickness = 0.0;

  // Create checkerboard floor (8x8 grid, 1 unit per square)
  for (int x = -4; x < 4; x++) {
    for (int z = -4; z < 4; z++) {
      Material &mat = ((x + z) % 2 == 0) ? checkWhite : checkDark;
      Shape *square = new Plane(
  {(float)x + 0.5f, -1.0f, (float)z + 0.5f},
  {0, 1, 0},
  mat,
  0.5f  // Half-size of 1x1 square
);
      scene.addShape(square);
    }
  }

// Glass sphere
  Material glass;
  glass.albedo = {0.99, 0.99, 0.99};
  glass.roughness = 0.0;
  glass.transmissive = true;
  glass.ior = 1.5;
  glass.absorption = 0.0;
  glass.thickness = 0.0;
  Shape *glassSphere = new Sphere({0, 0.8, -1.5}, 0.9, glass);
  scene.addShape(glassSphere);

  // Adding Lights
  Light l1;
  l1.color = {1, 1, 1};
  l1.energy = 10.0;
  l1.pos = {-3, 5, 2};
  scene.addLight(l1);

  Light l2;
  l2.color = {0.8, 0.9, 1};
  l2.energy = 5.0;
  l2.pos = {3, 4, 1};
  scene.addLight(l2);

  render.renderScene(cam, maxDepth, scene);
  saveImage("src/renders/render.png", render.W, render.H, render.framebuffer);

  return 0;
}
