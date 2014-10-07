//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Panagis Alisandratos on 10/4/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

Entity::Entity() {}
//note to self: need good constructor (that also creates sprite in body) if using dyn-mem methods

void Entity::Update(float elapsed) {
    //?
}

void Entity::Render() {
    //translatef in here so you dont have to include x, y???
    sprite.Draw(width, height, x, y); //if only calling draw what, then, is the point of render?
    //Draw without params?
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