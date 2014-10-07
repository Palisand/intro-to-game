//
//  Entity.h
//  Assignment4
//

#ifndef __Assignment4__Entity__
#define __Assignment4__Entity__

#include "SpriteSheet.h"
#include "Utility.h"
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Entity {
public:
    Entity();
    void Update(float elapsed);
    void Render();
    
    bool CollidesWith(Entity* entity);
    
    void FixedUpdate();
    
    SheetSprite sprite;
    GLuint textureId;
    float spriteIndex;
    int spriteCountX;
    int spriteCountY;
    float x;
    float y;
    
    float width;
    float height;
    float velocity_x;
    float velocity_y;
    float acceleration_x;
    float acceleration_y;
    float friction_x;
    float friction_y;
    float mass;
    bool isStatic;
    
    bool enableCollisions;
    
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    
    Utility util;
};

#endif /* defined(__Assignment4__Entity__) */
