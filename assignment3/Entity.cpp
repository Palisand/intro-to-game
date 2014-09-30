//
//  Entity.cpp
//  Assignment3
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"

Entity::Entity(float height, float width, GLuint textureID, float x, float y, float angle, bool visible, float x_scale, float y_scale) : height(height), width(width), textureID(textureID), x(x), y(y), angle(angle), visible(visible), x_scale(x_scale), y_scale(y_scale) {};

void Entity::DrawFromSprite() {
    if ( IsAlive() ) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glMatrixMode(GL_MODELVIEW);
        
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glScalef(x_scale, y_scale, 0.0);
        
        GLfloat quad[] = {
            -width * 0.5f, height * 0.5f,
            -width * 0.5f, -height * 0.5f,
            width * 0.5f, -height * 0.5f,
            width * 0.5f, height * 0.5f
        };
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //how to fade using alpha?
        
        glDrawArrays(GL_QUADS, 0, 4);
        glDisable(GL_TEXTURE_2D);
    }
}

void Entity::DrawFromSpriteSheet(int spriteCountX, int spriteCountY) {
    if ( IsAlive() ) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glMatrixMode(GL_MODELVIEW);
        
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glScalef(x_scale, y_scale, 0.0);
        
        GLfloat quad[] = {
            -width * 0.5f, height * 0.5f,
            -width * 0.5f, -height * 0.5f,
            width * 0.5f, -height * 0.5f,
            width * 0.5f, height * 0.5f
        };
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        float u = (float)(((int)spriteIndex) % spriteCountX) / (float)spriteCountX;
        float v = (float)(((int)spriteIndex) / spriteCountY) / (float)spriteCountY;
        float spriteWidth = 1.0/(float)spriteCountX;
        float spriteHeight = 1.0/(float)spriteCountY;
        
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
    }
}

void Entity::SetSize(float height, float width) {
    this->height = height;
    this->width = width;
}

void Entity::Scale(float magnitude) {
    x_scale *= magnitude;
    y_scale *= magnitude;
}

void Entity::Kill() {
    visible = false;
}

bool Entity::IsAlive() {
    return visible;
}

void Entity::Move(float elapsed) {
    x += dir_x * speed * elapsed;
    y += dir_y * speed * elapsed;
}