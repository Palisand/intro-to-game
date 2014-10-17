//
//  SidescrollerApp.h
//  Assignment5
//

#ifndef __Assignment5__SidescrollerApp__
#define __Assignment5__SidescrollerApp__

#include "Utility.h"
#include "Entity.h"
#include <vector>
#include <fstream>

#define GRAVITY -3

class SidescrollerApp {
public:
    SidescrollerApp();
    ~SidescrollerApp();
    
    void Init();
    void BuildLevel();
    bool UpdateAndRender();
    
    void Render();
    void RenderLevel();
    void Update(float elapsed);
    void FixedUpdate();
    
    //tile map methods
    bool readHeader(std::ifstream& stream);
    bool readLayerData(std::ifstream& stream);
    bool readEntityData(std::ifstream& stream);
    void placeEntity(std::string type, float placeX, float placeY);
    bool isSolid(unsigned char tile);
    
    //collision related
    void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
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
    
    unsigned char** levelData;
    int mapWidth;
    int mapHeight;
    float tileWidth = 0.1;
    float tileHeight = 0.1;
    int tileCountX = 8;
    int tileCountY = 3;
    GLuint tilesTextureId;
    
    std::vector<Entity*> entities;
    Entity plr;
    
    Utility util;
};


#endif /* defined(__Assignment5__SidescrollerApp__) */
