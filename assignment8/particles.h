//
//  particles.h
//  assignment8
//
//

#ifndef __Assignment8__particles__
#define __Assignment8__particles__

#include <vector>
#include "Vector.h"

struct Particle {
    Vector position;
    Vector velocity;
    float lifetime;
};

class ParticleEmitter {
public:
    ParticleEmitter(unsigned int particleCount);
    ParticleEmitter();
    ~ParticleEmitter();
    void Update(float elapsed);
    void Render();
    
    Vector position;
    Vector velocity;
    float velocityDeviation;
    Vector gravity;
    float maxLifetime;
    std::vector<Particle> particles;
};
#endif /* defined(__Assignement8__particles__) */
