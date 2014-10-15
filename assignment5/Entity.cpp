//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Panagis Alisandratos on 10/4/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

//Entity::Entity(float x, float y, float width, float height, GLuint textureId, float spriteIndex, int spriteCountX, int spriteCountY, bool isStatic) : x(x), y(y), width(width), height(height), isStatic(isStatic), velocity_x(0.0f), velocity_y(0.0f), acceleration_x(0.0f), acceleration_y(0.0f), friction_x(0.0f), friction_y(0.0f), mass(0.0f), enableCollisions(true), collidedTop(false), collidedBottom(false), collidedLeft(false), collidedRight(false) {
//    sprite = SheetSprite(textureId, spriteIndex, spriteCountX, spriteCountY);
//}

Entity::Entity() {}

void Entity::Update(float elapsed) {
    //?
}

void Entity::Render() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0f);
    sprite.Draw(width, height);
}

bool Entity::CollidesWith(Entity* entity) {
    return (
        x + (width * 0.5) > entity->x - (entity->width * 0.5) &&
        x - (width * 0.5) < entity->x + (entity->width * 0.5) &&
        y + (height * 0.5) > entity->y - (entity->height * 0.5) &&
        y - (height * 0.5) < entity->y + (entity->height * 0.5)
    );
}

void Entity::FixedUpdate() {
    if (!isStatic) {
        velocity_x = util.lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
        velocity_y = util.lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
        velocity_x += acceleration_x * FIXED_TIMESTEP;
        velocity_y += acceleration_y * FIXED_TIMESTEP;
    }
}