#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "vec3.hpp"

struct Material {
    Vec3 albedo;
    bool specular;

    Material(void) {
        this->albedo = Vec3(1., 1., 1.);
        this->specular = false;
    }

    Material(Vec3 albedo, bool specular) {
        this->albedo = albedo;
        this->specular = specular;
    }

    bool is_specular() {
        return this->specular;
    }

};

#endif
