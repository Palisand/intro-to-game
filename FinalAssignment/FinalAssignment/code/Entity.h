//
//  Entity.h
//  FinalAssignment
//

#ifndef __Assignment4__Entity__
#define __Assignment4__Entity__

#include "SheetSprite.h"
#include "Utility.h"
#include "Sound.h"
#include <vector>
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Entity {
public:

    Entity();
    ~Entity();
    virtual void Update(float elapsed);
    void Render();
    
    bool CollidesWith(Entity* entity, float x_offset = 0.0f, float y_offset = 0.0f);
    bool CollidesWithCenter(Entity* entity);
    
    void FixedUpdate(); //only with movement ... leave blank in base class
    
//    void SetPosition(float x, float y);
//    void SetSize(float width, float height);
//    void SetVelocityX(float velocity_x);
//    void SetVelocityY(float velocity_y);
//    void SetAccelerationX(float acceleration_x);
//    void SetAccelerationY(float acceleration_y);
    void Scale(float factor);
    
    SheetSprite sprite;
    
    float x;
    float y;
    float z;
    float width;
    float height;
    float x_scale = 1;
    float y_scale = 1;
    
    //change to vectors? yeah!
    float velocity_x;
    float velocity_y;
    float acceleration_x;
    float acceleration_y;
    float friction_x;
    float friction_y;
//    float mass;
    bool hasGrav;
    bool isStatic;
    bool isVisible;
    bool isAnimated;
    int animStart;
    int animEnd;
    int animLength;
    float animSpeed;
    bool enableCollisions;
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    std::vector<Sound> soundEffects;
    
    Utility util;
};

#endif /* defined(__Assignment4__Entity__) */
