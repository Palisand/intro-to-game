//
//  Utility.h
//  FinalAssignment
//

#ifndef __Assignment4__Utility__
#define __Assignment4__Utility__

#include <iostream>
#include <SDL_opengl.h>
#include <vector>

#define COLLISION_OFFSET 0.0001f
#define PI 3.14159265

class Entity;

class Utility {
public:
    enum Alignment {ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER};
    
    GLuint LoadTexture(const char* image_path, GLenum image_format = GL_BGRA, GLint texParam = GL_NEAREST);
    
    void DrawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y, Alignment align = ALIGN_LEFT);
    
    void DrawSprite(GLuint textureId, float width, float height, float x_scale, float y_scale, float x, float y, float index, int countX, int countY, bool oneSidedScaling = false);
    
    float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);
    
    float easeOutElastic(float from, float to, float time);
    
    float lerp(float from, float to, float t);
    
    float randRange(float min, float max);
};

#endif /* defined(__Assignment4__Utility__) */
