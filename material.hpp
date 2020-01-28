#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "vec3.hpp"

struct Material {
    Vec3 albedo;
    bool specular, transparent;

    Material(void) {
        this->albedo = Vec3(1., 1., 1.);
        this->specular = false;
        this->transparent = false;
    }

    Material(Vec3 albedo, bool specular) {
        this->albedo = albedo;
        this->specular = specular;
        this->transparent = false;
    }

    Material(Vec3 albedo, bool specular, bool transparent) {
        this->albedo = albedo;
        this->specular = specular;
        this->transparent = transparent;
    }

    // TODO use an enum instead of flags ?
    
    bool is_specular() {
        return this->specular;
    }

    bool is_transparent() {
        return this->transparent;
    }
};

#endif
