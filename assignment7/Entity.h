//
//  Entity.h
//  Assignment7
//

#ifndef __Assignment4__Entity__
#define __Assignment4__Entity__

#include "SheetSprite.h"
#include "Utility.h"
#include "Matrix.h"
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Entity {
public:

    Entity();
    
    void Update(float elapsed);
    void Render();
    void buildMatrix();
    void FixedUpdate();
    
    Matrix matrix;
    SheetSprite sprite;
    float x;
    float y;
    float scale_x;
    float scale_y;
    float rotation;
    float width;
    float height;
    float velocity_x;
    float velocity_y;
    float velocity_r;
    float acceleration_x;
    float acceleration_y;
    float acceleration_r;
    float friction_x;
    float friction_y;
    float friction_r;
    float mass;
    bool isStatic;
    bool enableCollisions;
    Utility util;
};

#endif /* defined(__Assignment4__Entity__) */
