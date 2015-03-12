//
//  Flag.h
//  FinalAssignment
//

#ifndef __FinalAssignment__Flag__
#define __FinalAssignment__Flag__

#include "Entity.h"
#include "Sound.h"
#include "FlagSpawnPoint.h"

class Flag : public Entity {
public:
    
    Flag();
    
    void Update(float elapsed);
    void Return();
    void Drop(float x, float y);
    void PickUp();
    void SetAnims(int animStart, int animLength, float animSpeed);
    
    FlagSpawnPoint spawnPoint;
};

#endif /* defined(__FinalAssignment__Flag__) */
