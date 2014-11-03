//
//  Entity.cpp
//  Assignment7
//

#include "Entity.h"
#include <math.h>

#define PI 3.141592654

Entity::Entity() {}

void Entity::Update(float elapsed) {
    //?
}

void Entity::Render() {
    
    buildMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(matrix.ml);
    
    sprite.Draw(width, height);
    
    glPopMatrix();
}

void Entity::buildMatrix() {
    Matrix scale;
    Matrix rotate;
    Matrix translate;
    
    scale.m[0][0] = scale_x;
    scale.m[1][1] = scale_y;
    scale.m[2][2] = 1.0f;
    
    rotate.m[0][0] = cos(rotation * PI / 180.0f);
    rotate.m[1][0] = -sin(rotation * PI / 180.0f);
    rotate.m[0][1] = sin(rotation * PI / 180.0f);
    rotate.m[1][1] = cos(rotation * PI / 180.0f);
    
    translate.m[3][0] = x;
    translate.m[3][1] = y;
    translate.m[3][2] = 0.0f;
    
    matrix = scale * rotate * translate;
}

void Entity::FixedUpdate() {
    if (!isStatic) {
        velocity_x = util.lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
        velocity_y = util.lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
        velocity_r = util.lerp(velocity_r, 0.0f, FIXED_TIMESTEP * friction_r);
        velocity_x += acceleration_x * FIXED_TIMESTEP;
        velocity_y += acceleration_y * FIXED_TIMESTEP;
        velocity_r += acceleration_r * FIXED_TIMESTEP;
        
        x += velocity_x * FIXED_TIMESTEP;
        y += velocity_y * FIXED_TIMESTEP;
        rotation += velocity_r * FIXED_TIMESTEP;
    }
}