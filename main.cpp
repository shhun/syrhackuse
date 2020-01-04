#define HEIGHT 700
#define WIDTH 1000
#define PI 3.141592654462
#define EPSILON 0.001
#define MAX_SPECULAR_RECURSION 10

#include <iostream>
#include <vector>
#include <math.h>
#include "utils.hpp"
#include "sphere.hpp"
#include "hit_record.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "scene.hpp"

using namespace std;

Vec3 reflect(Vec3 i, Vec3 n) {
    // @i incident vector
    // @n normal vector
    // reflects a ray @i on a specular surface
    // of normal @n
    return i - (n*2*(n*i));
}

Vec3 get_color(Scene scene, Ray r, int nb_rebound) {
    Vec3 color = Vec3(0., 0., 0.);
    Hit_record ht, shadow_ht;

    if (scene.hit(r, ht)) {
        Vec3 l_ = (scene.light- r.point_at_parameter(ht.t));
        Vec3 l = (scene.light- r.point_at_parameter(ht.t)).normalize();

        if (nb_rebound > 0 && ht.material.is_specular()) { // specular
            Ray reflected_ray(ht.p+ (ht.normal * EPSILON), reflect(r.direction.normalize(), ht.normal));
            color = get_color(scene, reflected_ray, nb_rebound -1);
        } 

            
        Ray shadow_ray(ht.p + (ht.normal * EPSILON), l);

        if (scene.hit(shadow_ray, shadow_ht) && (( (ht.p - shadow_ht.p).norm() < l.norm() ))) {
           ht.material.albedo = Vec3(0., 0., 0.); // shadow point (ie null albedo) 
        }

        double c = std::min(255., 3000 * std::max(0., l.dot(ht.normal)) / pow((ht.p- scene.light).norm(), 1));
        color.x +=  std::min(255., c * ht.material.albedo.x);     // RED
        color.y += std::min(255., c * ht.material.albedo.y); // GREEN
        color.z += std::min(255., c * ht.material.albedo.z); // BLUE
    }


    return color;
}

int main() {

    Scene scene;

    vector<unsigned char> img(WIDTH*HEIGHT*3, 0);
    Vec3 camera(0., 0., 5.);
    Hit_record ht, shadow_ht;
    Material mirror(Vec3(.0, .0, .0), true);
    Sphere sphere(Vec3(0., 0., -1.), 1);
    Sphere sphere_inv(Vec3(0., 3., -1.), 1, mirror);

     
    Sphere roof(Vec3(0., 1010., 0.), 995, Vec3(1., 1., 0.));
    Sphere ground(Vec3(0., -1000., 0.), 999, Vec3(1., 0., 0.));
    Sphere background(Vec3(0., 0., -1000.), 997, Vec3(0., 1., 0.));
    Sphere left_wall(Vec3(-1000., 0., 0.), 998, Vec3(.5, 0., 5.));
    Sphere right_wall(Vec3(1000., 0., 0.), 998, Vec3(0., 5., 5.));

    scene.objects.push_back(sphere_inv);
    scene.objects.push_back(sphere);
    //scene.objects.push_back(background);
    scene.objects.push_back(left_wall);
    scene.objects.push_back(right_wall);
    scene.objects.push_back(ground);
    //scene.objects.push_back(roof);

    float fov = 60 * (PI/180); //FOV in radiant

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            
            Vec3 direction(j - WIDTH/2. + .5, i - HEIGHT/2. + .5, -HEIGHT/(2*tan(fov/2.))); // Launch ray at pixel i, j
            Ray ray(camera, direction.normalize());

            Vec3 color = get_color(scene, ray, MAX_SPECULAR_RECURSION);
            img[((HEIGHT -i -1)*WIDTH + j) * 3] =  color.x;     // RED
            img[((HEIGHT -i -1)*WIDTH + j) * 3 + 1] = color.y; // GREEN
            img[((HEIGHT -i -1)*WIDTH + j) * 3 + 2] = color.z; // BLUE
        }
    }

    save_image("./out", &img[0], WIDTH, HEIGHT);

    return 0;
}
