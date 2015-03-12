//
//  Hill.h
//  FinalAssignment
//

#ifndef __FinalAssignment__Hill__
#define __FinalAssignment__Hill__

#include "Entity.h"

class Hill : public Entity {
public:
    Hill();
    
    void Update(float elapsed);
    
    
    
    int red;
    int blue;
    int white;
};

#endif /* defined(__FinalAssignment__Hill__) */
