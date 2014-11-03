//
//  AsteroidsApp.h
//  Assigment7
//

#ifndef __Assigment7__AsteroidsApp__
#define __Assigment7__AsteroidsApp__

#include "Utility.h"
#include "Entity.h"
#include "Vector.h"
#include <SDL.h>
#include <vector>

#define PI 3.141592654
#define MAX_ASTEROIDS 10

class AsteroidsApp {
public:
    AsteroidsApp();
    ~AsteroidsApp();
    
    void Init();
    void BuildLevel();
    bool UpdateAndRender();
    
    void Render();
    void Update(float elapsed);
    void FixedUpdate();
    
    bool CheckCollision(Entity* entity1, Entity* entity2);
    
    float checkPointCollisionX(float x, float y);
    float checkPointCollisionY(float x, float y);
    void handleCollisionX(Entity* entity);
    void handleCollisionY(Entity* entity);
    
private:
    bool done;
    float lastFrameTicks;
    SDL_Window* displayWindow;
    float timeLeftOver = 0.0f;
    const Uint8* keys;
    std::vector<Entity*> entities;
    Entity plr;
    Entity asteroids[MAX_ASTEROIDS];
    Utility util;
};

#endif /* defined(__Assigment7__AsteroidsApp__) */
