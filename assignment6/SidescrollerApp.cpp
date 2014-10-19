//
//  SidescrollerApp.cpp
//  Assignment5
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "SidescrollerApp.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;


SidescrollerApp::SidescrollerApp() {
    Init();
    done = false;
    lastFrameTicks = 0.0f;
    keys = SDL_GetKeyboardState(nullptr);
    
    BuildLevel();
}


SidescrollerApp::~SidescrollerApp() {
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(stepSound);
    Mix_FreeMusic(ambience);
    
    SDL_Quit();
}


void SidescrollerApp::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("A Sidescroller", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
//    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glOrtho(0.0, 2.66, -2.0, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
}


void SidescrollerApp::BuildLevel() {
    
    //SOUND
    jumpSound = Mix_LoadWAV("assets/sounds/jump.wav");
    stepSound = Mix_LoadWAV("assets/sounds/step.wav");
    ambience = Mix_LoadMUS("assets/sounds/Diogenes_In_Hell_-_04_-_Potential_Space.mp3");
    Mix_PlayMusic(ambience, -1);
    
    //PLAYER
    GLuint plrSpriteTexture = util.LoadTexture("assets/characters_no_shadow.png");
    plr.sprite = SheetSprite(plrSpriteTexture, 34, 12, 8);
    plr.x = 0.0f;
    plr.y = 0.0f;
    plr.width = 0.1f;
    plr.height = 0.1f;
    plr.isStatic = false;
    plr.enableCollisions = true;
    plr.velocity_x = 0.0f;
    plr.velocity_y = 0.0f;
    plr.acceleration_x = 0.0f;
    plr.acceleration_y = 0.0f;
    plr.friction_x = 7.0f;
    plr.friction_y = 2.0f;
    entities.push_back(&plr);

    //TILES
    tilesTextureId = util.LoadTexture("assets/platformertiles.png");
    
    ifstream infile("assets/sidescroller_tile_map.txt");
    string line;
    
    while (getline(infile, line)) {
        if (line == "[header]") {
            if (!readHeader(infile)) {
                return;
            }
        }
        else if (line == "[layer]") {
            readLayerData(infile);
        }
        else if (line == "[Object Layer 1]") {
            readEntityData(infile);
        }
    }
}


void SidescrollerApp::Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    float translateX = -plr.x + 1.33;
    float translateY = -plr.y - 1;
    
    //how did you use 0.0 instead of glOrtho values?!
//    if (translateY > 1.0) {
//        translateY = 1.0;
//    }
//    
//    if (translateX > -1.33) {
//        translateX = -1.33;
//    }
//    
//    if (translateX < -tileWidth * (mapWidth/2) - 1.87) {
//        translateX = -tileWidth * (mapWidth/2) - 1.87;
//    }
    
    if (translateY > 0.0) {
        translateY = 0.0;
    }
    if (translateX > 0.0) {
        translateX = 0.0;
    }
    if (translateX < -tileWidth * (mapWidth/2) - 0.66) {
        translateX = -tileWidth * (mapWidth/2) - 0.66;
    }

    glTranslatef(translateX, translateY, 0.0f);
    
    RenderLevel();
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->Render();
    }
    
    SDL_GL_SwapWindow(displayWindow);
}


void SidescrollerApp::RenderLevel() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    //    glTranslatef(-tileWidth * mapWidth/2, tileHeight * mapHeight/2, 0.0f);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tilesTextureId);
    
    vector<float> vertexData;
    vector<float> texCoordData;
    
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            float u = (float)(((int)levelData[y][x] - 1) % tileCountX) / (float)tileCountX; // - 1 to account for Tiled's index strangeness
            float v = (float)(((int)levelData[y][x] - 1) / tileCountX) / (float)tileCountY;
            float width = 1.0f / (float)tileCountX;
            float height = 1.0f / (float)tileCountY;
            
            if (levelData[y][x] != 0) {
                vertexData.insert(vertexData.end(), {
                    tileWidth * x,                  -tileHeight * y,
                    tileWidth * x,                  (-tileHeight * y) - tileHeight,
                    (tileWidth * x) + tileWidth,    (-tileHeight * y) - tileHeight,
                    (tileWidth * x) + tileWidth,    -tileHeight * y
                });
                
                texCoordData.insert(texCoordData.end(), {
                    u,          v,
                    u,          v + height,
                    u + width,  v + height,
                    u + width,  v
                });
            }
        }
    }
    
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, (GLsizei)vertexData.size()/2);
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
}


void SidescrollerApp::Update(float elapsed) {
    
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (!event.key.repeat) {
                if (event.key.keysym.scancode == SDL_SCANCODE_W && plr.collidedBottom == true) {
                    plr.velocity_y += 2.0f;
                    Mix_PlayChannel(2, jumpSound, 0);
                }
                if ((event.key.keysym.scancode == SDL_SCANCODE_D || event.key.keysym.scancode == SDL_SCANCODE_A) && plr.collidedBottom) {
                    plr.sprite.index = 35;
                }
            }
        }
    }
    
    if (keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A]) {
        plr.acceleration_x = 5.0f;
        
        if (plr.collidedBottom) {
            plr.sprite.index += 0.2f; //call animate() specific to player? !!!
            if (plr.sprite.index > 36) {
                plr.sprite.index -= 3;
            }
            Mix_PlayChannel(1, stepSound, 0);
        }
        else {
            plr.sprite.index = 35;
        }
        
        if (plr.sprite.x_scale < 0) {
            plr.sprite.FlipX();
        }
        
    }
    else if (keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) {
        plr.acceleration_x = -5.0f;
        
        if (plr.collidedBottom) {
            plr.sprite.index += 0.2f;
            if (plr.sprite.index > 36) {
                plr.sprite.index -= 3;
            }
            Mix_PlayChannel(1, stepSound, 0);
        }
        else {
            plr.sprite.index = 35;
        }
        
        if (plr.sprite.x_scale > 0) {
            plr.sprite.FlipX();
        }
        
    }
    else {
        plr.acceleration_x = 0.0f;
        if (plr.collidedBottom) {
            plr.sprite.index = 34;
        }
        else {
            plr.sprite.index = 35;
        }
    }
}


void SidescrollerApp::FixedUpdate() {
    
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->FixedUpdate();
        
        entities[i]->collidedTop = false;
        entities[i]->collidedBottom = false;
        entities[i]->collidedLeft = false;
        entities[i]->collidedRight = false;
        
        if (!entities[i]->isStatic) {
            entities[i]->velocity_y += GRAVITY * FIXED_TIMESTEP;
        }
    }
    
    // handle Y collisions
    for (int i = 0; i < entities.size(); i++) {
        entities[i]-> y += entities[i]->velocity_y * FIXED_TIMESTEP;
        if (!entities[i]->isStatic && entities[i]->enableCollisions) {
            handleCollisionY(entities[i]);
        }
    }
    
    // handle X collisions
    for (int i = 0; i < entities.size(); i++) {
        entities[i]-> x += entities[i]->velocity_x * FIXED_TIMESTEP;
        if (!entities[i]->isStatic && entities[i]->enableCollisions) {
            handleCollisionX(entities[i]);
        }
    }
    
}


bool SidescrollerApp::UpdateAndRender() {
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    float fixedElapsed = elapsed + timeLeftOver;
    if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    while (fixedElapsed >= FIXED_TIMESTEP) {
        fixedElapsed -= FIXED_TIMESTEP;
        FixedUpdate();
    }
    timeLeftOver = fixedElapsed;
    
    Update(elapsed);
    Render();
    return done;
}


bool SidescrollerApp::readHeader(ifstream& stream) {
    string line;
    mapWidth = -1;
    mapHeight = -1;
    
    while (getline(stream, line)) {

        if (line == "") {
            break;
        }
        
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if (key == "width") {
            mapWidth = atoi(value.c_str());
        }
        else if (key == "height") {
            mapHeight = atoi(value.c_str());
        }
//        else if (key == "tilewidth") {
//            tileWidth = atoi(value.c_str());
//        }
//        else if (key == "tileheight") {
//            tileHeight = atoi(value.c_str());
//        }
    }

    if (mapWidth == -1 || mapHeight == -1) {
        return false;
    }
    else {
        levelData = new unsigned char*[mapHeight];
        for (int i = 0; i < mapHeight; i++) {
            levelData[i] = new unsigned char[mapWidth];
        }
        return true;
    }
}


bool SidescrollerApp::readLayerData(std::ifstream& stream) {
    string line;
    
    while (getline(stream, line)) {
        if (line == "") {
            break;
        }
        
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if (key == "data") {
            for (int y = 0; y < mapHeight; y++) { //get rows
                getline(stream, line);
                istringstream lineStream(line);
                string tile;
                
                for (int x = 0; x < mapWidth; x++) { //get row values
                    getline(lineStream, tile, ',');
                    unsigned char val = (unsigned char)atoi(tile.c_str());
                    if (val > 0) {
                        //flare map file tiles are indexed from 1 (not 0)
                        levelData[y][x] = val; //- 1; ommitted because my tile layer did not have its first tile empty (see -1 implemented in Render()
                    }
                    else {
                        levelData[y][x] = 0;
                    }
                }
            }
        }
    }
    return true;
}


bool SidescrollerApp::readEntityData(std::ifstream& stream) {
    
    string line;
    string type;
    
    while(getline(stream, line)) {
        if (line == "") {
            break;
        }
        
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);

        if (key == "type") {
            type = value;
        }
        else if (key == "location") {
            istringstream lineStream(value);
            string xPosition, yPosition;
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
            
            float placeX = atoi(xPosition.c_str()) / 32 * tileWidth; //32 should be tileWidth/ tileHeight (get from readData, and replace others with TILE_SIZE)
            float placeY = atoi(yPosition.c_str()) / 32 * -tileHeight; //-TILE_SIZE why? because tileHeight should be pixels (size is 0.1);
            
            cout << placeX << ' ' << placeY;
            placeEntity(type, placeX, placeY);
        }
    }
    return true;
}


void SidescrollerApp::placeEntity(string type, float x, float y) {
    if (type == "Start") {
        plr.x = x;
        plr.y = y;
    }
}


bool SidescrollerApp::isSolid(unsigned char tile) {
    switch (tile) {
        case 1:
        case 2:
        case 3:
        case 5:
        case 9:
        case 10:
        case 11:
        case 13:
        case 17:
        case 18:
        case 19:
        case 21:
            return true;
            break;
        default:
            return false;
            break;
    }
}


void SidescrollerApp::worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY) {
//    *gridX = (int)((worldX + (tileWidth * mapWidth/2)) / tileWidth); //WORLD_OFFSET_X = tileWidth * mapWidth/2
//    *gridY = (int)((-worldY + (tileHeight * mapHeight/2)) / tileHeight); //WORLD_OFFSET_Y = tileHeight * mapHeight/2
    *gridX = (int)(worldX / tileWidth);
    *gridY = (int)(-worldY / tileHeight);
}


float SidescrollerApp::checkPointCollisionX(float x, float y) {
    int gridX, gridY;
    worldToTileCoordinates(x, y, &gridX, &gridY);
    
    if (gridX < 0 || gridX > mapWidth - 1 || gridY < 0 || gridY > mapHeight - 1) {
        return 0.0f;
    }
    
    if (isSolid(levelData[gridY][gridX])) {
        float xCoordinate = (gridX + 1) * tileWidth; //((gridX + 1) * tileWidth) - (tileWidth * mapWidth/2);
        return xCoordinate - x;
    }
    
    return 0.0f;
}


float SidescrollerApp::checkPointCollisionY(float x, float y) {
    int gridX, gridY;
    worldToTileCoordinates(x, y, &gridX, &gridY);

    if (gridX < 0 || gridX > mapWidth - 1 || gridY < 0 || gridY > mapHeight - 1) {
        return 0.0f;
    }
    
    if (isSolid(levelData[gridY][gridX])) {
        float yCoordinate = gridY * tileHeight;//((gridY) * tileHeight) - (tileHeight * mapHeight/2);//for no translation: (gridY * tileHeight); //TILE_SIZE 0.1f
        return -y - yCoordinate;
    }
    
    return 0.0f;
}


void SidescrollerApp::handleCollisionX(Entity* entity) {
    
    //left
    float penetration = checkPointCollisionX(entity->x - (entity->width * 0.5), entity->y);
    if (penetration != 0.0f) {
        entity->x += penetration/10; // divide by constant to prevent x-collision abruptness COLLISION_DAMPER
        entity->velocity_x = 0.0f;
        entity->collidedLeft = true;
    }
    
    //right
    penetration = checkPointCollisionX(entity->x + (entity->width * 0.5), entity->y);
    if (penetration != 0.0f) {
        entity->x += (penetration - tileWidth)/10;
        entity->velocity_x = 0.0f;
        entity->collidedRight = true;
    }
}


void SidescrollerApp::handleCollisionY(Entity* entity) {
    
    //bottom
    float penetration = checkPointCollisionY(entity->x, entity->y - (entity->height * 0.5));
    if (penetration != 0.0f) {
        entity->y += penetration;
        entity->velocity_y = 0.0f;
        entity->collidedBottom = true;
    }
    
    //top
    penetration = checkPointCollisionY(entity->x, (entity->y + entity->height * 0.5));
    if (penetration != 0.0f) {
        entity->y += (penetration - tileHeight) / 10;
        entity->velocity_y = 0.0f;
        entity->collidedTop = true;
    }
    
}

