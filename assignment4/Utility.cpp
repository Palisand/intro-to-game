//
//  Utility.cpp
//  Assignment4
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

void Utility::DrawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y) {
    
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    
    float texture_size = 1.0/16.0f;
    
    vector<float> vertexData;
    vector<float> texCoordData;
    vector<float> colorData;
    
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        
        colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        
        vertexData.insert(vertexData.end(), {((size+spacing) * i) + (-0.5f * size), 0.5f * size, ((size+spacing) * i) + (-0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), 0.5f * size});
        
        texCoordData.insert(texCoordData.end(), {texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +texture_size, texture_y + texture_size, texture_x + texture_size, texture_y});
    }
    
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0f);
    
    glDisableClientState(GL_COLOR_ARRAY);
}

float Utility::lerp(float v0, float v1, float t) {
    return (1.0 - t) * v0 + t * v1;
}

//float checkPointCollisionX(Entity* entity1, Entity* entity2) {
//    return fabsf((entity1->x - entity2->x) - (entity2->width * 0.5) - (entity1->width * 0.5));
//}
//
//float checkPointCollisionY(Entity* entity1, Entity* entity2) {
//    return fabsf((entity1->y - entity2->y) - (entity2->height * 0.5) - (entity1->height * 0.5));
//}

void Utility::handleCollisionX(Entity* entity, vector<Entity*> entities) {
    
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i] != entity && entities[i]->isStatic && entity->CollidesWith(entities[i])) {

            float x_penetration = fabs(entity->x - entities[i]->x) - (entity->width * 0.5) - (entities[i]->width * 0.5) - COLLISION_OFFSET; //how to get this working without an offset?!?!
            
            //left
            if (entity->x < entities[i]->x) {
                entity->x += x_penetration;
                entity->velocity_x = 0.0f;
                entity->collidedLeft = true;
            }
            //right
            if (entity->x > entities[i]->x) {
                entity->x -= x_penetration;
                entity->velocity_x = 0.0f;
                entity->collidedRight = true;
            }
        }
    }
}

void Utility::handleCollisionY(Entity* entity, vector<Entity*> entities) {
    
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i] != entity && entities[i]->isStatic && entity->CollidesWith(entities[i])) {

            float y_penetration = fabs(entity->y - entities[i]->y) - (entity->height * 0.5) - (entities[i]->height * 0.5) - COLLISION_OFFSET;
            
            //top
            if (entity->y < entities[i]->y) {
                entity->y += y_penetration;
                entity->velocity_y = 0.0f;
                entity->collidedTop = true;
            }
            //bottom
            if (entity->y > entities[i]->y) {
                entity->y -= y_penetration;
                entity->velocity_y = 0.0f;
                entity->collidedBottom = true;
            }
        }
    }
}