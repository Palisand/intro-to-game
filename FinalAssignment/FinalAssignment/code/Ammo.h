//
//  Ammo.h
//  FinalAssignment
//

#ifndef __FinalAssignment__Ammo__
#define __FinalAssignment__Ammo__

#define SPAWN_DELAY_AMMO 100

#include "Entity.h"
#include "Sound.h"

class Ammo : public Entity {
public:
    
    Ammo();
    
    void Update(float elapsed);
    void Respawn();
    void PickUp();
    void SetAnims(int animStart, int animLength, float animSpeed);
    
    float spawnDelay;
    std::vector<std::pair<float, float>> spawnPoints;
    std::pair<float, float> inUse;
};

#endif /* defined(__FinalAssignment__Ammo__) */
