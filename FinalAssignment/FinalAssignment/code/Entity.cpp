//
//  Entity.cpp
//  FinalAssignment
//

#include "Entity.h"

Entity::Entity() {}

Entity::~Entity() {
    for (Sound& sound: soundEffects) {
        sound.Quit();
    }
}

void Entity::Update(float elapsed) {
    return;
}

void Entity::Render() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    if (isVisible) {
        sprite.Draw(width * x_scale, height * y_scale);
    }
    glPopMatrix();
}

bool Entity::CollidesWith(Entity* entity, float x_offset, float y_offset) {
    return (
        x + (width * 0.5) > (entity->x + x_offset) - (entity->width * 0.5) &&
        x - (width * 0.5) < (entity->x + x_offset) + (entity->width * 0.5) &&
        y + (height * 0.5) > (entity->y + y_offset) - (entity->height * 0.5) &&
        y - (height * 0.5) < (entity->y + y_offset) + (entity->height * 0.5)
    );
}

bool Entity::CollidesWithCenter(Entity* entity) {
    return (
        x + (width * 0.5) > entity->x &&
        x - (width * 0.5) < entity->x &&
        y + (height * 0.5) > entity->y &&
        y - (height * 0.5) < entity->y 
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

void Entity::Scale(float factor) {
    x_scale *= factor;
    y_scale *= factor;
}