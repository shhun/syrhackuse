#ifndef HIT_HPP
#define HIT_HPP

#include "vec3.hpp"

struct Hit_record{
  double t;
  Vec3 p;
  Vec3 normal;
  Vec3 albedo;
  bool specular;

  Hit_record() {
    this->t = 0;
    Vec3 p_, normal_; 
    this->p = p_;
    this->normal = normal_; 

    // TODO initialize a material
  }
};

#endif
