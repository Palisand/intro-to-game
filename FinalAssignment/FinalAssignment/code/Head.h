//
//  Head.h
//  FinalAssignment
//

#ifndef __FinalAssignment__Head__
#define __FinalAssignment__Head__

#include "Entity.h"
#include "Sound.h"

class Head : public Entity {
public:
    
    Head();
    
    void Update(float elapsed);
    void Spawn(float x, float y);
    void PickUp();
    void SetAnims(int animStart, int animLength, float animSpeed);

};

#endif /* defined(__FinalAssignment__Head__) */
