//
//  Matrix.h
//  assigment7
//

#ifndef __assigment7__Matrix__
#define __assigment7__Matrix__

#include <iostream>

class Vector;

class Matrix {
public:
    Matrix();
    
    union {
        float m[4][4];
        float ml[16]; //glMultMatrixf() takes a 1-dimensional array
    };
    
    void identity();
    
    Matrix inverse();
    Matrix operator*(const Matrix &m2);
    Vector operator*(const Vector &v2);
    
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    
};

#endif /* defined(__assigment7__Matrix__) */
