//
//  SpaceInvadersApp.h
//  Assignment3
//

#ifndef __Assignment3__SpaceInvadersApp__
#define __Assignment3__SpaceInvadersApp__

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
    void InitGameLevel();
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
    void PlrShoot();
    
    bool PointCollision(Entity point, Entity obj);
    
    GLuint LoadTexture(const char* image_path, GLenum image_format = GL_BGRA, GLint texParam = GL_NEAREST);
    
    void DrawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
    
private:
    
    bool done;
    float lastFrameTicks;
    SDL_Window* displayWindow;
    SDL_Event event;
    enum gameState { STATE_TITLE_SCREEN, STATE_GAME_LEVEL, STATE_GAME_OVER };
    int state;
    const Uint8* keys;
    GLuint mainSpriteSheet;
    const char* fontSheetPath = "assets/fonts/pixel_font.png";
    GLuint fontSheetTexId;
    float screen_pulsate;
    float screen_red, screen_green, screen_blue;
    float title_red, title_green, title_blue;
    float enter_red, enter_green, enter_blue, enter_size;
    Entity* plr = new Entity(); //allocation on heap a bad idea?
    Entity* enemies = new Entity[MAX_ENEMIES];
    int enemiesTotal;
    Entity* plrShots = new Entity[MAX_PLR_SHOTS]; //change to projectile
    int plrShotIndex;
    float inc;
    int score;
    bool slow;
    float scoreSize = 0.1f;
    float scoreDamper;
};

#endif /* defined(__Assignment3__SpaceInvadersApp__) */
