//
//  Head.cpp
//  FinalAssignment
//

#include "Head.h"

Head::Head() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 0, 4096);
    
    for (int i = 1; i < 6; i++) {
        soundEffects.push_back("assets/sound/effects/collect_head" + std::to_string(i) + ".wav");
    }
}

void Head::Update(float elapsed) {
    sprite.index += animSpeed;
    if (sprite.index > animEnd + 1) {
        sprite.index = animStart;
    }
}

void Head::Spawn(float x, float y) {
    velocity_y = 3.0f;
    isVisible = true;
    this->x = x;
    this->y = y;
}

void Head::PickUp() {
    soundEffects[util.randRange(0, 5)].Play();
    isVisible = false;
}

void Head::SetAnims(int animStart, int animLength, float animSpeed) {
    this->animStart = animStart;
    animEnd = animStart + animLength;
    this->animLength = animLength;
    this->animSpeed = animSpeed;
}