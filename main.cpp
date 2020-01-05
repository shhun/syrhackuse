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

double random_value() {
    return rand() * .01;
}

Vec3 gamma_correction(Vec3 color) {
    return Vec3( 
            255 * pow(color.x/255., 1./2.2),
            255 * pow(color.y/255., 1./2.2),
            255 * pow(color.z/255., 1./2.2));
}

inline double square(double x) {
    return x * x;
}

Vec3 refract(Vec3 i, Vec3 n, double nair, double nsphere) {
    // @i incident vector
    // @n normal vector
    // reflects a ray @i on a specular surface
    // of normal @n

    auto left = i * (nair/nsphere);
    auto root = 1 - square(nair/nsphere)*(1. - square(i * n)); 
    auto right = n * ((nair/nsphere) * (i*n) + sqrt(root));

    return left - right;
}


Vec3 get_color(Scene scene, Ray r, int nb_rebound) {
    // returns the color of the object (or black if there is none)
    // hit by the @ray in @scene. Specular materials are allowed
    // @nb_rebound of light rebound.
    Vec3 color = Vec3(0., 0., 0.);
    Hit_record ht, shadow_ht;

    if (scene.hit(r, ht)) {
        Vec3 p = ht.p;
        Vec3 l_ = (scene.light- p);
        Vec3 l = (scene.light- p).normalize();

        if (nb_rebound > 0 && ht.material.is_specular()) { // specular
            Ray reflected_ray(ht.p+ (ht.normal * EPSILON), reflect(r.direction.normalize(), ht.normal));
            color = get_color(scene, reflected_ray, nb_rebound -1);
        } else if (nb_rebound > 0 && ht.material.is_transparent()) { // transparent
            double nair = 1.;
            double nsphere = .7;

            Vec3 i = r.direction.normalize();
            Vec3 n = ht.normal;

            if (r.direction * ht.normal > 0) { // leaving the sphere
                // swap refraction indices and invert local normal
                double tmp;
                tmp = nsphere;
                nsphere = nair;
                nair = tmp;
                n = Vec3() -n;
            }

            auto root = 1 - square(nair/nsphere)*(1. - square(i * n)); 
            if (root >= 0) {
                double nr = nair/nsphere;
                Vec3 refr_dir = i * nr - n * (i * n) - n * sqrt(root);
                Ray refracted_ray(ht.p +  (n * -EPSILON), refr_dir);
                color = get_color(scene, refracted_ray, nb_rebound -1);

            }
            
        } else {
    
         
            
        Ray shadow_ray(p + (ht.normal * EPSILON), l);

        double c = std::min(255., scene.I * std::max(0., l.dot(ht.normal)) / pow((p- scene.light).norm(), 1));

        if (scene.hit(shadow_ray, shadow_ht) &&   (( (p - shadow_ht.p).norm() < l_.norm() ))) {
            return color; // shadow pixel
        }

        color.x +=  std::min(255., c * ht.material.albedo.x);     // RED
        color.y += std::min(255., c * ht.material.albedo.y); // GREEN
        color.z += std::min(255., c * ht.material.albedo.z); // BLUE
        }
    }
    return gamma_correction(color);
}


int main() {

    //------------------------- Scene setup -----------------------------------------
    Scene scene;

    vector<unsigned char> img(WIDTH*HEIGHT*3, 0);
    Vec3 camera(0., 0., 20.);
    Hit_record ht, shadow_ht;
    Material mirror(Vec3(.0, .0, .0), true);
    Material glass(Vec3(1., .0, .0), false, true);
    //Sphere sphere(Vec3(0., 1., 0.), 1, Vec3(1., 1., 1.));
    Sphere sphere(Vec3(3., 0., 0), 2);
    Sphere sphere_inv(Vec3(-3., 0., 0.), 2, mirror);

     
    Sphere roof(Vec3(0., 1040., 0.), 1000, Vec3(1., 1., 0.));
    Sphere ground(Vec3(0., -1000.-20, 0.), 1000, Vec3(1., 0., 0.));
    Sphere background(Vec3(0., 0., -1000. - 40.), 1000, Vec3(0., .7, 0.));
    Sphere background_(Vec3(0., 0., 1000. + 40.), 1000, Vec3(0.3, .7, 0.));
    Sphere left_wall(Vec3(-1000.-20, 0., 0.), 1000, Vec3(.5, 0., 5.));
    Sphere right_wall(Vec3(1000.+20, 0., 0.), 1000, Vec3(0., 5., 5.));

    scene.objects.push_back(sphere_inv);
    scene.objects.push_back(sphere);
    scene.objects.push_back(background);
    scene.objects.push_back(background_);
    scene.objects.push_back(left_wall);
    scene.objects.push_back(right_wall);
    scene.objects.push_back(ground);
    scene.objects.push_back(roof);

    float fov = 60 * (PI/180); //FOV in radiant


    //------------------------- Compute image -----------------------------------------
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            
            Vec3 direction(j - WIDTH/2. + .5, i - HEIGHT/2. + .5, -HEIGHT/(2*tan(fov/2.))); // Launch ray at pixel i, j
            Ray ray(camera, direction.normalize());

            Vec3 color(0., 0., 0.);
            for (int i = 0; i < 3; i++) {
                Ray ray_(camera + Vec3(random_value(), random_value(), random_value()), direction.normalize());
                color = color + get_color(scene, ray, MAX_SPECULAR_RECURSION);
            }

            color = color * (1/3.);
            img[((HEIGHT -i -1)*WIDTH + j) * 3] =  color.x;     // RED
            img[((HEIGHT -i -1)*WIDTH + j) * 3 + 1] = color.y; // GREEN
            img[((HEIGHT -i -1)*WIDTH + j) * 3 + 2] = color.z; // BLUE
        }
    }

    save_image("./out", &img[0], WIDTH, HEIGHT);

    return 0;
}
