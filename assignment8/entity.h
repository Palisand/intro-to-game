//
//  entity.h
//
//

#ifndef ENTITY_H
#define ENTITY_H

namespace GameObj {
    
    class Entity {
    public:
        Entity();
        
        void Draw() const;
        
        void SetSize(float height, float width);
        
        float x;
        float y;
        float x_scale;
        float y_scale;
        float angle;
        float width;
        float height;
        
        int textureID;
        
        float speed;
        float max_speed;
        float dir_x;
        float dir_y;
    };
    
}

#endif /* defined(ENTITY_H) */
