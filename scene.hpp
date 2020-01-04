#ifndef SCENE_HPP
#define SCENE_HPP

#include "vec3.hpp"
#include "hit_record.hpp"
#include "sphere.hpp"
#include <vector>

using namespace std;

struct Scene {
    vector<Sphere> objects;
    Vec3 light;
 
    Scene (void) {
        this->objects = *(new vector<Sphere>);
        this->objects.reserve(100);       // Meh
        this->light = Vec3(0., 10., 10.); //TODO fixed light for now
    };

    bool hit(Ray ray, Hit_record &ht) {
        Hit_record tmp_ht;
        double min_t = 1000000000; // Meh
        bool hit = false;

        // Find the closest hit according to tmp_ht.t
        for (auto sphere : this->objects) {
            hit = sphere.hit(ray, tmp_ht) || hit;
        
            if (hit && tmp_ht.t < min_t) {
                sphere.hit(ray, ht);
                min_t = tmp_ht.t;
            }
        }

        return hit;
    }

};

#endif
