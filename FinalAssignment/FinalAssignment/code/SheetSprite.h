//
//  SheetSprite.h
//  FinalAssignment
//

#ifndef __Assignment4__SpriteSheet__
#define __Assignment4__SpriteSheet__

#include <SDL_opengl.h>

class SheetSprite {
public:
    SheetSprite();
    SheetSprite(GLuint textureId, float index, int countX, int countY); //uniform sheet
    SheetSprite(GLuint textureId, float u, float v, float width, float height); //non-uniform sheet
    
    void Draw(float objWidth, float objHeight);
    void FlipX();
    void Scale(float factor);
    
    GLuint textureId;
    
    float x_scale = 0.5f;
    float y_scale = 0.5f;
    
    float u;
    float v;
    float width;
    float height;
    
    float index;
    int countX;
    int countY;
};

#endif /* defined(__Assignment4__SpriteSheet__) */
