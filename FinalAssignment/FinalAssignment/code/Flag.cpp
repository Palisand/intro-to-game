//
//  Flag.cpp
//  FinalAssignment
//

#include "Flag.h"

Flag::Flag() {

    x = spawnPoint.x;
    y = spawnPoint.y;
    width = 0.1f;
    height = 0.1f;
    hasGrav = false;
    isStatic = true;
    isVisible = true;
    enableCollisions = false;
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 0, 4096);
    
    for (int i = 1; i < 6; i++) {
        soundEffects.push_back("assets/sound/effects/collect_head" + std::to_string(i) + ".wav");
    }
    soundEffects.push_back(Sound("assets/sound/effects/flag_return1.wav"));
}

void Flag::Update(float elapsed) {
    sprite.index += animSpeed;
    if (sprite.index > animEnd) {
        sprite.index = animStart;
    }
    
}

void Flag::Return() {
    x = spawnPoint.x;
    y = spawnPoint.y;
    spawnPoint.flagged = true;
    isVisible = true;
}

void Flag::Drop(float x, float y) {
    this->x = x;
    this->y = y;
    isVisible = true;
}

void Flag::PickUp() {
    spawnPoint.flagged = false;
    soundEffects[util.randRange(0,5)].Play();
    isVisible = false;
}

void Flag::SetAnims(int animStart, int animLength, float animSpeed) {
    this->animStart = animStart;
    animEnd = animStart + animLength;
    this->animLength = animLength;
    this->animSpeed = animSpeed;
}