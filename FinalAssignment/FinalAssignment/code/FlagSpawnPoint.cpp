//
//  FlagSpawnPoint.cpp
//  FinalAssignment
//

#include "FlagSpawnPoint.h"

FlagSpawnPoint::FlagSpawnPoint() {
    
    width = 0.1f;
    height = 0.1f;
    hasGrav = false;
    isStatic = true;
    isVisible = true;
    enableCollisions = false;
}

void FlagSpawnPoint::Update(float elapsed) {
    if (flagged) {
        sprite.index = animStart;
    }
    else {
        sprite.index = animEnd;
    }
}