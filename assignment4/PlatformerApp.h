//
//  PlatformerApp.h
//  Assignment4
//

#ifndef __Assignment4__PlatformerApp__
#define __Assignment4__PlatformerApp__

#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <vector>

#include "Utility.h"
#include "Entity.h"

#define MAX_TILES 102
#define MAX_COINS 5
#define GRAVITY -9

class PlatformerApp {
public:
    
    PlatformerApp();
    ~PlatformerApp();
    
    void Init();
    void BuildLevel();
    bool UpdateAndRender();
    
    // no game states for this one
    void Render();
    void Update(float elasped);
    void FixedUpdate();
    
private:
    bool done;
    float lastFrameTicks;
    SDL_Window* displayWindow;
    float timeLeftOver = 0.0f;
    const Uint8* keys;
    std::vector<Entity*> entities;
    Entity plr;
    Entity tiles[MAX_TILES];
    Entity coins[MAX_COINS];
    
    Utility util;
};

#endif /* defined(__Assignment4__PlatformerApp__) */
