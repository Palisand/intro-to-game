//
//  FlagSpawnPoint.h
//  FinalAssignment
//

#ifndef __FinalAssignment__FlagSpawnPoint__
#define __FinalAssignment__FlagSpawnPoint__

#include "Entity.h"

class FlagSpawnPoint : public Entity {
public:
    FlagSpawnPoint();
    
    void Update(float elapsed);
    
    bool flagged;
};

#endif /* defined(__FinalAssignment__FlagSpawnPoint__) */
