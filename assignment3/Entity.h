//
//  Entity.h
//
//  Created by Panagis Alisandratos on 9/28/14.
//

#ifndef __Entity__
#define __Entity__

#include <SDL_opengl.h>

class Entity {
public:
    Entity(bool visible = true);
    
    void Draw() const;
    void DrawFromSpriteSheet(int index, int spriteCountX, int spriteCountY) const;
    
    void SetSize(float height, float width);
    
    float x;
    float y;
    float x_scale;
    float y_scale;
    float angle;
    float width;
    float height;
    
    int textureID;
    
    float speed;
    float max_speed;
    float dir_x;
    float dir_y;
    
    float health;
    bool visible;
};

#endif /* defined(__Entity__) */
