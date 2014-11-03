//
//  Vector.cpp
//  assignment7
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
