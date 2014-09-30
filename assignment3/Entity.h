//
//  Entity.h
//  Assignment3
//

#ifndef __Assignment3__Entity__
#define __Assignment3__Entity__

#include <SDL_opengl.h>

class Entity {
public:
    
    //extended constructor
    Entity(float height = 0.1, float width = 0.1, GLuint textureID = 0, float x = 0.0f, float y = 0.0f, float angle = 0.0f, bool visible = true, float x_scale = 1.0f, float y_scale = 1.0f);
    
    void DrawFromSprite();
    void DrawFromSpriteSheet(int spriteCountX, int spriteCountY);
    
    //conveniency methods
    void SetSize(float height, float width);
    void Scale(float magnitude);
    void Kill(); //based on visibility (change this later)
    bool IsAlive(); //based on visibility (change this later)
    void Move(float elapsed);
    
    /* all attributes currently public for ease of use */
    GLuint textureID;
    float spriteIndex;
    int startSpriteIndex;
    int maxSpriteIndex;
    float animSpeed;
    float alpha;
    bool visible;
    float x;
    float y;
    float x_scale;
    float y_scale;
    float width;
    float height;
    float angle;
    float dir_x;
    float dir_y;
    float speed;
    float max_speed;
    float health;
};

#endif /* defined(__Assignment3__Entity__) */
