//
//  Particles.cpp
//  FinalAssignment
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "particles.h"
#include  <random>

#include <iostream>

ParticleEmitter::ParticleEmitter() {}

ParticleEmitter::ParticleEmitter(unsigned int particleCount) {
    
    particles.resize(particleCount);
    
//    maxLifetime = 2.0f;
//    minLifetime = 0.0f;
    
    particleSize = 0.0f;
    particleDecayRate = 1.0f;
    
    for (int i = 0; i < particles.size(); i++) {
//        particles[i].lifetime = ((float)rand() / (float)RAND_MAX) * maxLifetime - 1;
//        std::cout << particles[i].lifetime << std::endl;
        particles[i].lifetime = 0.0f;
    }
    
    gravity = Vector(0.0f, -5.0f, 0.0f);
    velocity = Vector(0.0f, 0.0f, 0.0f);
    velocityDeviation = Vector(1.0f, 1.0f, 0.0f);
}

//ParticleEmitter::~ParticleEmitter() {
//    
//}

void ParticleEmitter::Update(float elapsed) {
    
    for (int i = 0; i < particles.size(); i++) {
        particles[i].velocity += (gravity * elapsed);
        particles[i].position += (particles[i].velocity * elapsed);
        
//        particles[i].lifetime += elapsed;
//        
//
//        if (particles[i].lifetime > maxLifetime) {
//            particles[i].lifetime = 0.0f;
//            particles[i].velocity = velocity;
//            particles[i].position = position;
//        }
    }
    
    particleSize -= particleDecayRate;
    
}


void ParticleEmitter::EmitX(int amount, bool fromLeft) {
    for (int i = 0; i < amount; i++) {
        particleSize = 12.0f;
        particles[i].velocity = velocity;
        particles[i].position.y = position.y;
        if (fromLeft) {
            particles[i].position.x = position.x + 0.05f;
            particles[i].velocity.x = -velocityDeviation.x * ((float)rand() / (float)RAND_MAX);
            particles[i].velocity.y = velocityDeviation.y * ((float)rand() / (float)RAND_MAX);
        }
        else {
            particles[i].position.x = position.x - 0.05f;
            particles[i].velocity.x = velocityDeviation.x * ((float)rand() / (float)RAND_MAX);
            particles[i].velocity.y = velocityDeviation.y * ((float)rand() / (float)RAND_MAX);
        }
        
    }
}

void ParticleEmitter::EmitY(int amount, bool fromBottom) {
    for (int i = 0; i < amount; i++) {
        particleSize = 12.0f;
        particles[i].velocity = velocity;
        particles[i].position = position;
        if (fromBottom) {
            particles[i].velocity.y = -velocityDeviation.y * ((float)rand() / (float)RAND_MAX);
            particles[i].velocity.x = velocityDeviation.x * ((float)rand() / (float)RAND_MAX) - 0.5f;
        }
        else {
            particles[i].velocity.y = velocityDeviation.y * ((float)rand() / (float)RAND_MAX);
            particles[i].velocity.x = velocityDeviation.x * ((float)rand() / (float)RAND_MAX) - 0.5f;
        }
    }
}

void ParticleEmitter::EmitYoffsetX(int amount, bool fromBottom, bool fromLeft, float offset) {
    for (int i = 0; i < amount; i++) {
        particleSize = 12.0f;
        particles[i].velocity = velocity;
        particles[i].position.y = position.y;
        if (fromLeft) {
            particles[i].position.x = position.x + offset;
        }
        else {
            particles[i].position.x = position.x - offset;
        }
        if (fromBottom) {
            particles[i].velocity.y = -velocityDeviation.y * ((float)rand() / (float)RAND_MAX);
            particles[i].velocity.x = velocityDeviation.x * ((float)rand() / (float)RAND_MAX) - 0.5f;
        }
        else {
            particles[i].velocity.y = velocityDeviation.y * ((float)rand() / (float)RAND_MAX);
            particles[i].velocity.x = velocityDeviation.x * ((float)rand() / (float)RAND_MAX) - 0.5f;
        }
    }
}


void ParticleEmitter::Render() {
    std::vector<float> particleVertices;

    for(int i = 0; i < particles.size(); i++) {
        particleVertices.push_back(particles[i].position.x);
        particleVertices.push_back(particles[i].position.y);
    }
    
    glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    
//    glEnable(GL_POINT_SMOOTH);
    glPointSize(particleSize); //will affect all particles!?!?!?
    glDrawArrays(GL_POINTS, 0, (GLsizei)particleVertices.size()/2);
}