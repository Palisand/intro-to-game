//
//  Particles.h
//  FinalAssignment
//

#ifndef __Assignment8__particles__
#define __Assignment8__particles__

#include <vector>
#include "Vector.h"

struct Particle { //try using quads and include a size attribute
    Vector position;
    Vector velocity;
    float lifetime;
};

class ParticleEmitter {
public:
    ParticleEmitter(unsigned int particleCount);
    ParticleEmitter();
//    ~ParticleEmitter();
    void Update(float elapsed);
    void Render();
    void EmitX(int amount, bool fromLeft);
    void EmitY(int amount, bool fromBottom);
    void EmitYoffsetX(int amount, bool fromBottom, bool fromLeft, float offset);
    
    Vector position;
    Vector velocity;
    Vector velocityDeviation;
    Vector gravity;
    float maxLifetime;
    float minLifetime;
    float particleSize;
    std::vector<Particle> particles;
    std::vector<float> particleColors; //red and blue (fade to white?)
    float particleDecayRate;
};
#endif /* defined(__Assignement8__particles__) */
