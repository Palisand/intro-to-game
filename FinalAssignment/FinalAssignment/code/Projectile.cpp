//
//  Projectile.cpp
//  FinalAssignment
//

#include "Projectile.h"
#include "math.h"

#include <iostream>
using namespace std;

Projectile::Projectile() {
    emitter = ParticleEmitter(100);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 4096);
    
    for (int i = 1; i < 6; i++) {
        soundEffects.push_back("assets/sound/effects/bullet_hit" + std::to_string(i) + ".wav");
        soundEffects[i-1].SetVolume(50);
    }
}

void Projectile::Update(float elapsed) {
    
    emitter.position.x = x;
    emitter.position.y = y;
    
    if (velocity_x == 0) {
        if (isVisible) {
            emitter.EmitX(10, (sprite.x_scale > 0) ? true : false);
            soundEffects[util.randRange(0, 5)].Play();
        }
        isVisible = false;
    }
    
    if (sprite.index < muzzleFlashIndex + 1) {
        sprite.index += 0.3;
    }
    
}

