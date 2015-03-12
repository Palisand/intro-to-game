//
//  Ammo.cpp
//  FinalAssignment
//

#include "Ammo.h"

Ammo::Ammo() {
    spawnDelay = -1;
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 4096);
    
    for (int i = 1; i < 6; i++) {
        soundEffects.push_back("assets/sound/effects/pickup" + std::to_string(i) + ".wav");
    }
}

void Ammo::Update(float elapsed) {
    if (spawnDelay > -1) {
        spawnDelay--;
    }
    
    if (spawnDelay == 0) {
        Respawn();
    }
    
    sprite.index += animSpeed;
    if (sprite.index > animEnd + 1) {
        sprite.index = animStart;
    }
}

void Ammo::Respawn() {
    
    int coordinates = util.randRange(0, spawnPoints.size() - 1);
    
    while (spawnPoints[coordinates].first == inUse.first && spawnPoints[coordinates].second == inUse.second) {
        coordinates = util.randRange(0, spawnPoints.size() - 1);
    }
    
    x = spawnPoints[coordinates].first;
    y = spawnPoints[coordinates].second;
    
    isVisible = true;
}

void Ammo::PickUp() {
    soundEffects[util.randRange(0, 5)].Play();
    isVisible = false;
    x = 99;
    spawnDelay = SPAWN_DELAY_AMMO;
}

void Ammo::SetAnims(int animStart, int animLength, float animSpeed) {
    this->animStart = animStart;
    animEnd = animStart + animLength;
    this->animLength = animLength;
    this->animSpeed = animSpeed;
}