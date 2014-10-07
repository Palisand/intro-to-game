//
//  Utility.h
//  Assignment4
//

#ifndef __Assignment4__Utility__
#define __Assignment4__Utility__

#include <iostream>
#include <SDL_opengl.h>
#include <vector>

#define COLLISION_OFFSET 0.0001f

class Entity;

class Utility {
public:
    
    GLuint LoadTexture(const char* image_path, GLenum image_format = GL_BGRA, GLint texParam = GL_NEAREST);
    
    void DrawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
    
    float lerp(float v0, float v1, float t);
    
//    float checkPointCollisionX(Entity* entity1, Entity* entity2);
//    float checkPointCollisionY(Entity* entity1, Entity* entity2);
    
    void handleCollisionX(Entity* entity, std::vector<Entity*> entities);
    void handleCollisionY(Entity* entity, std::vector<Entity*> entities);
};

#endif /* defined(__Assignment4__Utility__) */
