//
//  Vector.h
//  assigment8
//

#ifndef __assigment7__Vector__
#define __assigment7__Vector__

#include <stdio.h>

class Vector {
public:
    Vector();
    Vector(float x, float y, float z);
    
    float length();
    void normalize(); //into unit vector
    
    Vector operator*(float scalar);
    void operator+=(const Vector& v);
    
    float x;
    float y;
    float z;
};

#endif /* defined(__assigment7__Vector__) */
