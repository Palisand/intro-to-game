//
//  Projectile.h
//  FinalAssignment
//

#ifndef __NYUCodebase__Projectile__
#define __NYUCodebase__Projectile__

#include "Entity.h"
#include "Particles.h"
#include "Sound.h"

class Projectile : public Entity {
public:
    Projectile();
    
    void Update(float elapsed);
    
    int muzzleFlashIndex;
    ParticleEmitter emitter;
};

#endif /* defined(__NYUCodebase__Projectile__) */
