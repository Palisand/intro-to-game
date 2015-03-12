//
//  Application.h
//  FinalAssignment
//

#ifndef __Assignment5__Application__
#define __Assignment5__Application__

#include "Utility.h"
#include "Player.h"
#include "Ammo.h"
#include "Flag.h"
#include "FlagSpawnPoint.h"
//#include "Hill.h";
#include "Particles.h"
#include "PerlinNoise.h"
#include "Sound.h"
#include <vector>
#include <fstream>

#define PI 3.141592654
#define GRAVITY -9
#define GL_ORTHO_X 1.33f
#define GL_ORTHO_Y 1.00f
#define SCALE 0.7
#define MAX_SCALE 1.3
#define SCALE_DAMP 5
#define COLLISION_DAMP 1.1 //set to 0.1 for cool effect on pushing towards collision
#define TILE_SIZE 0.1f
#define MAX_BULLETS 2
#define MIN_SCORE 5
#define SHOOT_SCORE 10
#define COLLECT_SCORE 8
#define UV_OFFSET 0.001f
#define TITLE_MENU_ITEM_COUNT 6

class Application {
public:
    Application();
    ~Application();
    
    void Init();
    
    void BuildGlobalAssets();
    void BuildHeadHunterLevel();
    void BuildCaptureTheFlagLevel();
    void BuildKingOfTheHillLevel();
    
    void ResetGlobalEntities();
    void BuildMap(const std::string& tileMapPath, int type);
    void SpawnGlobalEntities();
    bool UpdateAndRender();
    
    void Render();
    void RenderTitleScreen();
    void RenderInstructions();
    void RenderHeadHunterLevel();
    void RenderCaptureTheFlagLevel();
    void RenderKingOfTheHillLevel();
    void RenderGameOver();
    
    void RenderCamera();
    void RenderBackground();
    void RenderLevel();
    
    void Update(float elapsed);
    void UpdateTitleScreen(float elapsed);
    void UpdateInstructions(float elapsed);
    void UpdateHeadHunterLevel(float elapsed);
    void UpdateCaptureTheFlagLevel(float elapsed);
    void UpdateKingOfTheHillLevel(float elapsed);
    void UpdateGameOver(float elapsed);
    
    void UpdateInput();
    void UpdateCamera(float elapsed);
    
    void FixedUpdate();
    
    void HandlePlayerButtonAction(SDL_Event& event, Player& plr);
    
    //tile map methods
    bool readHeader(std::ifstream& stream);
    bool readLayerData(std::ifstream& stream, const std::string& tileLayer);
    bool readEntityData(std::ifstream& stream);
    void placeEntity(std::string type, float placeX, float placeY);
    void storeLocation(std::string type, float x, float y);
    bool isSolid(unsigned char tile);
    
    //collision related
    void PlrOnPlr();
    void PlrOnAmmo();
    void PlrOnHead();
    void PlrOnHill();
    void PlrOnFlag();
    void PlrOnFlagSpawn();
    void PlrOnMine(bool dropHead, bool dropFlag);
    void PlrOnBullet(bool dropHead, bool dropFlag);
    void BulletOnBullet();
    void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
    float checkPointCollisionX(float x, float y);
    float checkPointCollisionY(float x, float y);
    void handleCollisionX(Entity* entity);
    void handleCollisionY(Entity* entity);
    
private:
    bool done;
    float lastFrameTicks;
    SDL_Window* displayWindow;
    float timeLeftOver;
    const Uint8* keys;
    SDL_Event event;
    
    enum gameState {STATE_TITLE_SCREEN, STATE_INSTRUCTIONS_SCREEN, STATE_HEAD_HUNTER_LEVEL, STATE_CAPTURE_THE_FLAG_LEVEL, STATE_KING_OF_THE_HILL_LEVEL, STATE_GAME_OVER };
    int state;
    
    enum gameType {TYPE_HEAD_HUNTER, TYPE_CAPTURE_THE_FLAG, TYPE_KING_OF_THE_HILL};
    int type;
    
    SDL_Joystick* plr1Controller;
//    SDL_Haptic* plr1Haptic;
    
    SDL_Joystick* plr2Controller;
    
    float xMove1, xMove2;
    float menuMove;
    
//    SDL_HapticEffect shootRumble; //declaring before SDL_Init is bad juju
//    int shootRumbleId;
//    SDL_HapticEffect deathRumble;
    
    GLuint spriteSheetTexId;
    int sheetCountX;
    int sheetCountY;
    
    GLuint fontSheetTexId;
    float p1r, p1g, p1b;
    float p2r, p2g, p2b;
    
    unsigned char** levelData;
    int mapWidth;
    int mapHeight;
    float tileWidth;
    float tileHeight;
    
    float scale;
    float centerX;
    float centerY;
    float translateX;
    float translateY;
    
    float screenShakeValX = 0;
    float screenShakeSpeed = 50;
    float screenShakeIntensity = 0.01;
    
    bool notReady;
    
    std::vector<Entity*> entities;
    std::vector<ParticleEmitter*> emitters;
    Player plr1;
    Player plr2;
    Ammo plr1Ammo;
    Ammo plr2Ammo;
    Flag plr1Flag;
    Flag plr2Flag;
    Entity theHill;
    float KOTH_barScale;
    int timer;
    
    std::string plr1FlagStatus;
    std::string plr2FlagStatus;
    
    std::vector<Sound> soundEffects;
    std::vector<std::string> songs;
    Mix_Music* music;
    
    std::vector<float> menuItemSizes;
    int currMenuSel;
    
    float bgR, bgG, bgB, bgA;
    float bgFlashTime = 0;
    float shakeTime = 0;
    float foo = 0;
    float perlinVal;
    float perlinDeathMult = 25;
    
    Utility util;
};


#endif /* defined(__Assignment5__Application__) */