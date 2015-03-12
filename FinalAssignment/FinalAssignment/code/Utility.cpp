//
//  Utility.cpp
//  FinalAssignment
//

#include "Utility.h"
#include "Entity.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
using namespace std;

GLuint Utility::LoadTexture(const char* image_path, GLenum image_format, GLint texParam) {
    
    SDL_Surface *surface = IMG_Load(image_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, image_format, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParam);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

void Utility::DrawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y, Alignment align) {
    
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    switch (align) {
        case ALIGN_LEFT:
            glTranslatef(x, y, 0.0);
            break;
        case ALIGN_RIGHT:
            glTranslatef(x - ( (text.size() - 1) * (size+spacing) ), y, 0.0);
            break;
        case ALIGN_CENTER:
            glTranslatef(x - ( (text.size() - 1) * (size+spacing) ) / 2, y, 0.0);
            break;
        default:
            break;
    }
    
    float texture_size = 1.0/16.0f;
    
    vector<float> vertexData;
    vector<float> texCoordData;
    vector<float> colorData;
    
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        
        colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size
        });
        
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y
        });
    }
    
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0f);
    
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Utility::DrawSprite(GLuint textureId, float width, float height, float x_scale, float y_scale, float x, float y, float index, int countX, int countY, bool oneSidedScaling) {
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (oneSidedScaling) {
        glTranslatef(x - (width * x_scale), y - (height * y_scale), 0.0f);
    }
    else {
        glTranslatef(x, y, 0.0f);
    }
    
    GLfloat quad[] = {
        -width * x_scale, height * y_scale,
        -width * x_scale, -height * y_scale,
        width * x_scale, -height * y_scale,
        width * x_scale, height * y_scale
    };
    
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    float u, v, spriteWidth, spriteHeight;
    u = (float)(((int)index) % countX) / (float)countX;
    v = (float)(((int)index) / countX) / (float)countY;
    spriteWidth = 1.0/(float)countX;
    spriteHeight = 1.0/(float)countY;
    
    GLfloat quadUVs[] = {
        u, v,
        u, v + spriteHeight,
        u + spriteWidth, v + spriteHeight,
        u + spriteWidth, v
    };
    
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
}

float Utility::mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
    float retVal = dstMin + ((value - srcMin)/(srcMax-srcMin) * (dstMax-dstMin));
    if(retVal < dstMin) {
        retVal = dstMin;
    }
    if(retVal > dstMax) {
        retVal = dstMax;
    }
    return retVal;
}

float Utility::easeOutElastic(float from, float to, float time) {
    float p = 0.3f;
    float s = p/4.0f;
    float diff = (to - from);
    return from + diff + (diff*pow(2.0f,-10.0f*time) * sin((time-s)*(2*PI)/p));
}

float Utility::lerp(float from, float to, float time) {
    return (1.0 - time)*from + time*to;
}

float Utility::randRange(float min, float max) {
    return (float(rand()) / float(RAND_MAX) * (max - min)) + min;
}