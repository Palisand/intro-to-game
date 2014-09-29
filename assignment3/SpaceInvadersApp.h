//
//  SpaceInvadersApp.h
//
//  Created by Panagis Alisandratos on 9/28/14.
//

#ifndef __SpaceInvadersApp__
#define __SpaceInvadersApp__

#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include "Entity.h"

#define MAX_PLR_SHOTS 10
#define MAX_ENEMIES 15
#define MAX_ENEMY_SHOTS 20

class SpaceInvadersApp {
public:
    
    SpaceInvadersApp();
    ~SpaceInvadersApp();
    
    void Init();
    void SetupGameLevel();
    bool UpdateAndRender();
    
    void Render();
    void RenderTitleScreen();
    void RenderGameLevel();
    void RenderGameOver();
    
    void Update(float elapsed);
    void UpdateTitleScreen(float elapsed);
    void UpdateGameLevel(float elapsed);
    void UpdateGameOver(float elapsed);
    
    //helper functions
    void PlrShoot(); //should be a method inside a Entity derived Player class
    bool PointCollision(Entity point, Entity obj); //where is the best place for this helper function?
    GLuint LoadTexture(const char* image_path, GLenum image_format = GL_RGBA);
    void DrawText(int fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a);
    
private:
    
    bool done;
    float lastFrameTicks;
    SDL_Window* displayWindow;
    SDL_Event event;
    enum gameState { STATE_TITLE_SCREEN, STATE_GAME_LEVEL, STATE_GAME_OVER };
    int state;
    const Uint8* keys;
    Entity plr;
    int plrShotIndex;
    float plrSpriteIndex;
    Entity plrShots[MAX_PLR_SHOTS];
    Entity enemies[MAX_ENEMIES];
    Entity enemyShots[MAX_ENEMY_SHOTS];
    const char* spriteSheetPath;
    GLuint fontTexId;
};


#endif /* defined(__SpaceInvadersApp__) */
