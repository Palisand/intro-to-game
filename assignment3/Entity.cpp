//
//  Entity.cpp
//
//  Created by Panagis Alisandratos on 9/28/14.
//

#include "Entity.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

Entity::Entity(bool visible) : x_scale(1), y_scale(1), angle(0), visible(visible) {}

void Entity::Draw() const {
    if (visible) {
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
        
        glDrawArrays(GL_QUADS, 0, 4);
        glDisable(GL_TEXTURE_2D);
    }
}

void Entity::DrawFromSpriteSheet(int index, int spriteCountX, int spriteCountY) const {
    if (visible) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
        float v = (float)(((int)index) / spriteCountY) / (float)spriteCountY;
        float spriteWidth = 1.0/(float)spriteCountX;
        float spriteHeight = 1.0/(float)spriteCountY;
        
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