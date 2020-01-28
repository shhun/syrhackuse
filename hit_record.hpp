#ifndef HIT_HPP
#define HIT_HPP

#include "vec3.hpp"
#include "material.hpp"

struct Hit_record{
  double t;
  Vec3 p;            // Hit point of the ray on a surface
  Vec3 normal;       // Normal of the surface at point @p;
  Material material; // Material of the surface hit;

  Hit_record() {
    this->t = 0;
    this->p = Vec3();
    this->normal = Vec3();
    this->material = Material();
  }
};

#endif
