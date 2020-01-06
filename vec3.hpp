#ifndef VEC3_HPP
#define VEC3_HPP
#include <cmath>

struct Vec3 {
    double x, y, z;

    Vec3 (double a, double b, double c) {
        this->x = a;
        this->y = b;
        this->z = c;
    };

    Vec3 () {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    // Addition for vectors
    Vec3 add(Vec3 v) {
        return Vec3 (this->x + v.x, this->y + v.y, this->z + v.z);
    }

    // Addition for vectors
    Vec3 operator+(const Vec3 &v) const {
        return Vec3 (this->x + v.x, this->y + v.y, this->z + v.z);
    }

    // Subtraction for vectors
    Vec3 operator-(const Vec3 &v) const {
        return Vec3 (this->x - v.x, this->y - v.y, this->z - v.z);
    }


    // Right scalar multiplication for vectors
    Vec3 operator*(const double f) const {
        return Vec3 (this->x * f, this->y * f, this->z * f);
    }

    // Right division by a scalar
    Vec3 operator/(double f) {
        return Vec3 (this->x / f, this->y / f, this->z / f);
    }

    // Scalar product for vectors
    double operator*(Vec3 &v) {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }

    double dot(Vec3 v) {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }


    // Compute the Euclidean norm
    double norm() {
        return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
    }

    // Return a normalized copy of the input vector
    Vec3 normalize() {
        double n = this->norm();
        return Vec3(this->x / n, this->y / n, this->z / n);
    }

    // Outer product by a scalar
    Vec3 outer(double f){
      return Vec3(this->x * f, this->y * f, this->z * f);
    }

    // cross product
    Vec3 cross(Vec3 v) {
        return Vec3(
                this->y * v.z - this->z * v.y,
                this->z * v.x - this->x * v.z,
                this->x * v.y - this->y * v.x
                );
    }
};

#endif
