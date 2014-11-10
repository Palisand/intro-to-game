//
//  particles.cpp
//  assignment8
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "particles.h"
#include  <random>

ParticleEmitter::ParticleEmitter(unsigned int particleCount) {
    particles.resize(particleCount);
    
    maxLifetime = 2.0f;
    for (int i = 0; i < particles.size(); i++) {
        particles[i].lifetime = ((float)rand() / (float)RAND_MAX) * maxLifetime;
    }
    
    gravity = Vector(0.0f, -5.0f, 0.0f);
    velocity = Vector(0.0f, 1.0f, 0.0f);
    velocityDeviation = 1;
}

ParticleEmitter::~ParticleEmitter() {
    
}

void ParticleEmitter::Update(float elapsed) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i].velocity += (gravity * elapsed);
        particles[i].position += (particles[i].velocity * elapsed);
        
        particles[i].lifetime += elapsed;
        
        velocity.y = ((float)rand() / (float)RAND_MAX) * velocityDeviation;
        
        if (particles[i].lifetime > maxLifetime) {
            particles[i].lifetime = 0.0f;
            particles[i].velocity = velocity;
            particles[i].position = position;
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
    
    glPointSize(5.0f);
    glDrawArrays(GL_POINTS, 0, particleVertices.size()/2);
}