//
//  SpaceInvadersApp.cpp
//  Assignment3
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "SpaceInvadersApp.h"
#include "Entity.h"

#include <vector>
#include <string>
#include <iostream>
using namespace std;

#define PI 3.14159265

SpaceInvadersApp::SpaceInvadersApp() { //constructor - setup all vars
    Init();
    state = 0;
    done = false;
    lastFrameTicks = 0.0f;
    keys = SDL_GetKeyboardState(nullptr);

    InitGameLevel();
    
    inc = 0.0f;
    
    screen_red = 0.0f;
    screen_green = 0.0f;
    screen_blue = 0.0f;
    fontSheetTexId = LoadTexture(fontSheetPath);
    title_red = 1.0f;
    title_green = 1.0f;
    title_blue = 1.0f;
    enter_red = 1.0f;
    enter_green = 1.0f;
    enter_blue = 1.0f;
    enter_size = 0.1f;
}

void SpaceInvadersApp::InitGameLevel() {
    

    score = 0;
    scoreDamper = 0.0f;
    enemiesTotal = MAX_ENEMIES;
    slow = true;
    
    screen_pulsate = 0.0f;
    
    mainSpriteSheet = LoadTexture("assets/ms_pacman_spritesheet.png");
    plr->textureID = mainSpriteSheet;
    plr->speed = 1.5f;
    plr->x = 0.0f;
    plr->y = -0.9f;
    plr->spriteIndex = 2;
    
    float enemy_x = -0.6f;
    float enemy_y = 0.8f;
    int enemyStartSpriteIndices[4] = {18, 26, 34, 42};

    for (int i = 0; i < MAX_ENEMIES; i++) {
        
        int dir_x_switch = rand() % 2;
        
        enemies[i].visible = true;
        enemies[i].textureID = mainSpriteSheet;
        float randIndex = enemyStartSpriteIndices[rand() % 4];
        enemies[i].startSpriteIndex = randIndex;
        enemies[i].maxSpriteIndex = randIndex + 2;
        enemies[i].spriteIndex = enemies[i].startSpriteIndex;
        enemies[i].animSpeed = 0.1f;
        enemies[i].health = 2.0f;
        
        enemies[i].dir_y = -0.05f;
        enemies[i].dir_x = dir_x_switch ? float(rand() % 2 + 15) / 10 : -float(rand() % 2 + 15) / 10;
        enemies[i].speed = 0.005f;
        
        enemies[i].x = enemy_x;
        enemies[i].y = enemy_y;
        
        if ( (i+1) % 5 ) {
            enemy_x += 0.3f;
        }
        else {
            enemy_x = -0.6f;
            enemy_y -= 0.2f;
        }
    }

    for (int i = 0; i < MAX_PLR_SHOTS; i++) {
        plrShots[i].visible = false;
    }
}

SpaceInvadersApp::~SpaceInvadersApp() {
    SDL_Quit();
}

void SpaceInvadersApp::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Space Invaders?", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

/*************************************
 *------------ RENDERING ------------*
 *************************************/

void SpaceInvadersApp::Render() {
    glClearColor(screen_red, screen_green, screen_blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch(state) {
        case STATE_TITLE_SCREEN:
            RenderTitleScreen();
            break;
        case STATE_GAME_LEVEL:
            RenderGameLevel();
            break;
        case STATE_GAME_OVER:
            RenderGameOver();
            break;
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void SpaceInvadersApp::RenderTitleScreen() {
    DrawText(fontSheetTexId, "SPACE INVADERS?", 0.1f, -0.01f, title_red, title_green, title_blue, 1.0f, -1.0f, 0.3f);
    DrawText(fontSheetTexId, "This title screen would", 0.1f, -0.02f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f);
    DrawText(fontSheetTexId, "absolutely love it if you", 0.1f, -0.02f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -0.15f);
    DrawText(fontSheetTexId, "pressed", 0.1f, -0.02f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -0.3f);
    DrawText(fontSheetTexId, "ENTER", enter_size, -0.02f, enter_red, enter_green, enter_blue, 1.0f, -0.35f, -0.3f);
    DrawText(fontSheetTexId, "( Creator not liable for any injury via epileptic siezure )", 0.04f, -0.006f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -0.6f);
}

void SpaceInvadersApp::RenderGameLevel() {
    DrawText(fontSheetTexId, "SCORE: ", 0.1f, -0.02f, 1.0f, 1.0f, 1.0f, 1.0f, -1.25f, 0.9f);
    DrawText(fontSheetTexId, to_string(score), scoreSize, -0.02f, 1.0f, 1.0f, 1.0f, 1.0f, -0.75f, 0.9f);
    
    //won
    if (enemiesTotal == 0) {
        DrawText(fontSheetTexId, "YOU WIN!", 0.2f, -0.01f, title_red, title_green, title_blue, 1.0f, -0.6f, 0.3f);
    }

    title_red = cos(inc/10 * PI / 180);
    title_green = cos(inc/2 * PI / 180);
    title_blue = cos(inc * PI / 180);
    
    plr->DrawFromSpriteSheet(8, 8);
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].DrawFromSpriteSheet(8, 8);
    }
    
    for (int i = 0; i < MAX_PLR_SHOTS; i++) {
        plrShots[i].SetSize(0.03, 0.03);
        plrShots[i].DrawFromSprite();
    }
}

void SpaceInvadersApp::RenderGameOver() {
    DrawText(fontSheetTexId, "GAME OVER", 0.2f, -0.01f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.3f);
    DrawText(fontSheetTexId, "SCORE: " + to_string(score), 0.1f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.1f);
    DrawText(fontSheetTexId, "Give it another go", 0.1f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -0.2f);
    DrawText(fontSheetTexId, "(Press ENTER)", 0.1f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -0.4f);
}

/*************************************
 *------------ UPDATING -------------*
 *************************************/

void SpaceInvadersApp::Update(float elapsed) {
    
    inc += 100;
    
    switch(state) {
        case STATE_TITLE_SCREEN:
            UpdateTitleScreen(elapsed);
            break;
        case STATE_GAME_LEVEL:
            UpdateGameLevel(elapsed);
            break;
        case STATE_GAME_OVER:
            UpdateGameOver(elapsed);
            break;
    }
    
}

void SpaceInvadersApp::UpdateTitleScreen(float elapsed) {
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && !event.key.repeat) {
                state = 1;
            }
        }
    }
    
    if (keys[SDL_SCANCODE_RETURN]) {
        enter_red = cos(inc/2 * PI / 180);
        enter_green = cos(inc/3 * PI / 180);
        enter_blue = cos(inc/4 * PI / 180);
        enter_size = 0.2f;
    }
    
    title_red = cos(inc/10 * PI / 180);
    title_green = cos(inc/2 * PI / 180);
    title_blue = cos(inc * PI / 180);
    
}

void SpaceInvadersApp::UpdateGameLevel(float elapsed) {
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat) {
                //plr->Shoot();
                PlrShoot();
            }
        }
    }
    
    //plr movement
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        plr->dir_x = -1.0f;
        if (plr->x < -1.33f) {
            plr->x = 1.33f;
        }
    }
    else if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        plr->dir_x = 1.0f;
        if (plr->x > 1.33f) {
            plr->x = -1.33f;
        }
    }
    else {
        plr->dir_x = 0.0f;
    }

    plr->Move(elapsed);
    
    if (plr->spriteIndex < 2) {
        plr->spriteIndex += 0.2f;
    }
    
    for (int i = 0; i < MAX_PLR_SHOTS; i++) {
        plrShots[i].Move(elapsed);
    }
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        //should be calling Update method for Entity derived class Enemy, ain't nobody got time fo' dat
        
        //initial speed boost
        if (enemies[i].health < 2 && slow) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                enemies[j].speed = 0.5f;
            }
            slow = false;
        }
        
        enemies[i].Move(elapsed);
        enemies[i].Move(elapsed);
        
        //initial animation
        enemies[i].spriteIndex += enemies[i].animSpeed;
        if (enemies[i].spriteIndex > enemies[i].maxSpriteIndex) {
            enemies[i].spriteIndex = enemies[i].startSpriteIndex;
        }
        
        //screen edge collision
        if (enemies[i].x > 1.33f) {
            enemies[i].x = 1.33f;
            enemies[i].dir_x = -enemies[i].dir_x;
        }
        if (enemies[i].x < -1.33f) {
            enemies[i].x = -1.33f;
            enemies[i].dir_x = -enemies[i].dir_x;
        }
        
        if (enemies[i].IsAlive()) {
            switch((int)enemies[i].health) {
                case 0: //die
                    enemies[i].Kill();
                    enemiesTotal--;
                    break;
                case 1: //panic
                    enemies[i].speed = 0.6f; //possibly use a base speed and speed = baseSpeed + ?;
                    enemies[i].animSpeed = 0.2f;
                    enemies[i].startSpriteIndex = 12;
                    enemies[i].maxSpriteIndex = 15;
                    break;
            }
        }
        
        if (enemiesTotal == 1) {
            enemies[i].speed = 1.0f;
        }
        
        scoreDamper += 0.001f;
        
        //damage intake
        for (int j = 0; j < MAX_PLR_SHOTS; j++) {
            if ( plrShots[j].visible && enemies[i].IsAlive() && PointCollision(plrShots[j], enemies[i]) ) {
                enemies[i].health -= 1.0f;
                plrShots[j].visible = false;
                //score update
                scoreSize = 0.2f;
                if (enemies[i].health == 1) {
                    score += 100 / scoreDamper;
                }
                else if (enemiesTotal == 1){
                    score += 300 / scoreDamper;
                }
                else {
                    score += 150 / scoreDamper;
                }
                
                screen_pulsate = 10.0f;
                
            }
        }
        
        //game over
        if (enemies[i].y <= plr->y && enemies[i].IsAlive()) {
            //plr-death sequence
            state = 2;
        }
    }
    
    if (scoreSize > 0.1f) {
        scoreSize -= 0.01f;
    }
    else {
        scoreSize = 0.1f;
    }
    
    if (screen_pulsate || enemiesTotal == 0) {
        screen_pulsate--;
        screen_red = cos(inc/10 * PI / 180);
        screen_green = cos(inc/2 * PI / 180);
        screen_blue = cos(inc * PI / 180);
    }
    else {
        screen_pulsate = 0;
        screen_red = 0.0f;
        screen_green = 0.0f;
        screen_blue = 0.0f;
    }
    
}

void SpaceInvadersApp::UpdateGameOver(float elapsed) {
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && !event.key.repeat) {
            state = 1;
            InitGameLevel();
        }
    }
    
}

/*************************************
 *------ UPDATING & RENDERING -------*
 *************************************/

bool SpaceInvadersApp::UpdateAndRender() {
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    Update(elapsed);
    Render();
    return done;
}

/*************************************
 *-------- HELPER FUNCTIONS ---------*
 *************************************/

void SpaceInvadersApp::PlrShoot() {
    plr->spriteIndex = 0;
    
    plrShots[plrShotIndex].visible = true;
    plrShots[plrShotIndex].x = plr->x;
    plrShots[plrShotIndex].y = plr->y;
    plrShots[plrShotIndex].dir_y = 1.0f;
    plrShots[plrShotIndex].speed = 2.0f;
    
    plrShotIndex++;
    if (plrShotIndex > MAX_PLR_SHOTS - 1) {
        plrShotIndex = 0;
    }
}

bool SpaceInvadersApp::PointCollision(Entity point, Entity obj) {
    
    return point.x < (obj.x + (obj.width * 0.5)) &&
    point.x > (obj.x - (obj.width * 0.5)) &&
    point.y < (obj.y + (obj.height * 0.5)) &&
    point.y > (obj.y - (obj.height * 0.5));
}

GLuint SpaceInvadersApp::LoadTexture(const char* image_path, GLenum image_format, GLint texParam) {
    SDL_Surface *surface = IMG_Load(image_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, image_format, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParam);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

void SpaceInvadersApp::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y) {
    
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    
    float texture_size = 1.0/16.0f;
    
    vector<float> vertexData;
    vector<float> texCoordData;
    vector<float> colorData;
    
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        
        colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        
        vertexData.insert(vertexData.end(), {((size+spacing) * i) + (-0.5f * size), 0.5f * size, ((size+spacing) * i) + (-0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), 0.5f * size});
        
        texCoordData.insert(texCoordData.end(), {texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +texture_size, texture_y + texture_size, texture_x + texture_size, texture_y});
    }
    
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0f);
    
    glDisableClientState(GL_COLOR_ARRAY);
}