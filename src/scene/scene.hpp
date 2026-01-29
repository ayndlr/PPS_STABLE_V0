#pragma once
#include "../shapes/shapes.hpp"
#include <iostream>
#include <limits>
#include <vector>

class Scene {
  std::vector<Shape *> shapes;
  std::vector<Light> lights;

public:
  void addLight(Light &light) { lights.push_back(light); }
  
  std::vector<Light>& getLights(){
    return this->lights;
  }
  
  void addShape(Shape *&shape) { shapes.push_back(shape); }

  bool intersectScene(Ray &ray, Hit &hit) {
    bool anyHit = false;
    float tClosest = std::numeric_limits<float>::infinity();
    Hit tempHit;

    // iterate all shapes
    for (Shape *shape : shapes) {
      Hit thisHit;
      if (shape->intersect(ray, thisHit)) {
        if (thisHit.tHit < tClosest) {
          tClosest = thisHit.tHit;
          tempHit = thisHit;
          anyHit = true;
        }
      }
    }

    if (anyHit) {
      hit = tempHit; // only copy the closest hit
    }

    return anyHit;
  }
};
