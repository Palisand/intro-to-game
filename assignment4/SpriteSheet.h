//
//  SpriteSheet.h
//  Assignment4
//

#ifndef __Assignment4__SpriteSheet__
#define __Assignment4__SpriteSheet__

#include <SDL_opengl.h>

class SheetSprite {
public:
    SheetSprite();
    SheetSprite(GLuint textureId, float spriteIndex, int spriteCountX, int spriteCountY); //uniform sheet
    SheetSprite(GLuint textureId, float u, float v, float width, float height); //non-uniform sheet
    
    void Draw(float objWidth, float objHeight, float x = 0.0f, float y = 0.0f, float scale = 0.5f); //sprite_index?
    
    float scale;
    GLuint textureId;
    float u;
    float v;
    float width;
    float height;
};

#endif /* defined(__Assignment4__SpriteSheet__) */
