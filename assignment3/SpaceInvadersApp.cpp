//
//  SpaceInvadersApp.cpp
//
//  Created by Panagis Alisandratos on 9/28/14.
//

#include "SpaceInvadersApp.h"
#include "Entity.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

SpaceInvadersApp::SpaceInvadersApp() { //constructor
    Init();
    state = 0;
    done = false;
    lastFrameTicks = 0.0f;
    
    keys = SDL_GetKeyboardState(nullptr);
    plrShotIndex = 0;
    plrSpriteIndex = 2;
    spriteSheetPath = "sprites/ms_pacman_spritesheet.gif";
    fontTexId = LoadTexture("sprites/pixel_font.png");
    SetupGameLevel(); //this happens before the UpdateAndRender loop so as not to reset any values, how can we set this up when we enter a game state (only once, but all game state functions/methods are called within the loop!)
}

SpaceInvadersApp::~SpaceInvadersApp() {
    SDL_Quit();
}

void SpaceInvadersApp::Init() { //what used to come first in main
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Space Invaders: Ms. Pacman Vs Ghosts on Acid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void SpaceInvadersApp::Render() {
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
    DrawText(fontTexId, "This Title Screen wants you to Press ENTER", 0.05, 0, 1, 1, 1, 1);
}

void SpaceInvadersApp::RenderGameLevel() {
    plr.SetSize(0.1, 0.1);
    plr.DrawFromSpriteSheet(plrSpriteIndex, 8, 8);
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].SetSize(0.1, 0.1);
        enemies[i].DrawFromSpriteSheet(rand() % 32 + 16, 8, 8);
    }
    
    for (int i = 0; i < MAX_PLR_SHOTS; i++) {
        plrShots[i].SetSize(0.03, 0.03);
        plrShots[i].Draw();
    }

}

void SpaceInvadersApp::RenderGameOver() {
    DrawText(fontTexId, "Game Over", 0.1, 0, 1, 1, 1, 1);
}

void SpaceInvadersApp::Update(float elapsed) {
    
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
        else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
            state++;
        }
    }
}

void SpaceInvadersApp::UpdateGameLevel(float elapsed) {
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat) {
                plrSpriteIndex = 0;
                PlrShoot();
            }
        }
    }
    
    //player keystrokes and screen wrapping
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        plr.dir_x = -1.0f;
        if (plr.x < -1.33f) {
            plr.x = 1.33f;
        }
    }
    else if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        plr.dir_x = 1.0f;
        if (plr.x > 1.33f) {
            plr.x = -1.33f;
        }
    }
    else {
        plr.dir_x = 0.0f;
    }
    
    if (enemies[4].x > 1.33f || enemies[0].x < -1.33f) {
        for (int i = 0; i < MAX_ENEMIES; i++) {
            enemies[i].dir_x = -enemies[i].dir_x;
        }
    }
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        //should be calling Update method for an Entity derived class Enemy
        enemies[i].x += enemies[i].dir_x * enemies[i].speed * elapsed;
        enemies[i].y += enemies[i].dir_y * enemies[i].speed * elapsed;
        
        if (enemies[i].health <= 0.0f) {
            enemies[i].visible= false; //HOW TO PROPERLY DELETE? I have tried creating an array of Entity pointers (which doesn't show errors for 'delete enemies[i]', but I get a bad-access runtime error whenever I access any members of the instance i.e. 'enemies[i]->health')
        }
        //bullet-enemy collision and damage intake (should be a function with a shorter name)
        for (int j = 0; j < MAX_PLR_SHOTS; j++) {
            if ( plrShots[j].visible && enemies[i].visible && PointCollision(plrShots[j], enemies[i]) ) {
                enemies[i].health -= 1.0f;
                plrShots[j].visible = false;
            }
        }
        
        if (enemies[i].y <= plr.y) {
            state++;
        }
    }
    
    plr.x += plr.dir_x * plr.speed * elapsed;
    
    for (int i = 0; i < MAX_PLR_SHOTS; i++) {
        plrShots[i].y += plrShots[i].dir_y * plrShots[i].speed * elapsed;
    }
    
    if (plrSpriteIndex < 2) {
        plrSpriteIndex += 0.2;
    }
    
}

void SpaceInvadersApp::UpdateGameOver(float elapsed) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
    }
}

bool SpaceInvadersApp::UpdateAndRender() {
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    Update(elapsed);
    Render();
    return done;
}

void SpaceInvadersApp::SetupGameLevel() {
    GLuint ms_p_sprites = LoadTexture(spriteSheetPath);
    
    plr.x = 0.0f; //if this were more intricate, a SetupPlayer/Entity functions should be written
    plr.y = -0.8f;
    plr.speed = 1.5f;
    plr.textureID = ms_p_sprites;
    
    float enemy_x = -0.4f;
    float enemy_y = 0.8f;
    int dir_x_switch = rand() % 2;
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        
        enemies[i].textureID = ms_p_sprites;
        enemies[i].health = 3.0f;
        
        enemies[i].dir_y = -0.05f;
        enemies[i].dir_x = dir_x_switch ? 1.0f : -1.0f;
        enemies[i].speed = 1.0f;
        
        enemies[i].x = enemy_x;
        enemies[i].y = enemy_y;
        
        if ( (i+1) % 5 ) {
            enemy_x += 0.2f;
        }
        else {
            enemy_x = -0.4f;
            enemy_y -= 0.2f;
        }
    }
    
    for (int i = 0; i < MAX_PLR_SHOTS; i++) {
        plrShots[i].visible = false;
    }
}


void SpaceInvadersApp::PlrShoot() {
    //rapid-fire power-up???
    plrShots[plrShotIndex].x = plr.x;
    plrShots[plrShotIndex].y = plr.y;
    plrShots[plrShotIndex].dir_y = 1.0f;
    plrShots[plrShotIndex].speed = 2.0f;
    plrShots[plrShotIndex].visible = true;
    
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

GLuint SpaceInvadersApp::LoadTexture(const char* image_path, GLenum image_format) {
    SDL_Surface *surface = IMG_Load(image_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, image_format, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

void SpaceInvadersApp::DrawText(int fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
    
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-1.0, 0.0, 0.0);
    
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
    glDrawArrays(GL_QUADS, 0, text.size() * 4);
    
    glDisableClientState(GL_COLOR_ARRAY);
}