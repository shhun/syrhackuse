#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

struct Ray{
  Vec3 origin;
  Vec3 direction;

  Ray(Vec3 origin, Vec3 direction){
    this->origin = origin;
    this->direction = direction;
  }

  Vec3 point_at_parameter(double t){
    return origin.add(direction.outer(t));
  }
};

#endif
