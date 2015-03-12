//
//  Vector.cpp
//  FinalAssignment
//

#include "Vector.h"
#include <cmath>

Vector::Vector() {}

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector::length() {
    return std::sqrt(powf(x,2) + powf(y,2) + powf(z,2));
}

void Vector::normalize() {
    float length = this->length();
    
    x /= length;
    y /= length;
    z /= length;
}

Vector Vector::operator*(float scalar) {
    Vector v;
    v.x = x * scalar;
    v.y = y * scalar;
    v.z = z * scalar;
    
    return v;
}

void Vector::operator+=(const Vector& v) {
    x += v.x;
    y += v.y;
    z += v.z;
}