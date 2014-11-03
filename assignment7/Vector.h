//
//  Vector.h
//  assigment7
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
    
    float x;
    float y;
    float z;
};

#endif /* defined(__assigment7__Vector__) */
