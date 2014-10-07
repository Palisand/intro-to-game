//
//  PlatformerApp.cpp
//  NYUCodebase
//
//  Created by Panagis Alisandratos on 10/4/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "PlatformerApp.h"
#include "Entity.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
using namespace std;

PlatformerApp::PlatformerApp() {
    Init();
    done = false;
    lastFrameTicks = 0.0f;
    keys = SDL_GetKeyboardState(nullptr);
    
    BuildLevel();
}

PlatformerApp::~PlatformerApp() {
    SDL_Quit();
}

void PlatformerApp::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("A Platformer", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void PlatformerApp::BuildLevel() {
    
    GLuint appSpritesTexture = util.LoadTexture("assets/platformer_spritesheet.png");
    int spriteCountX = 30;
    int spriteCountY = 30;
    
    //PLAYER
    //how to create player in one line (with an appropriate constructor) and have it not deconstruct when function ends?! tried using dynamic memory (with 'new') and it works but I'm not sure if this is the best course of action
    plr.spriteIndex = 19;
    plr.sprite = SheetSprite(appSpritesTexture, plr.spriteIndex, spriteCountX, spriteCountY);
    plr.x = 0.0f; plr.y = 0.0f; plr.width = 0.1f; plr.height = 0.1f;
    plr.isStatic = false;
    plr.enableCollisions = true;
    plr.velocity_x = 0.0f; plr.velocity_y = 0.0f;
    plr.acceleration_x = 0.0f; plr.acceleration_y = 0.0f;
    plr.friction_x = 5.0f; plr.friction_y = 5.0f;
    
    entities.push_back(&plr);
    
    //TILES
    //Why is spacing in between tiles inconsistent?
    vector<pair<float, float>> tileCoordinates;
    float startFromLeft = -1.0f;
    float endAtRight = 1.0f;
    while (startFromLeft < endAtRight + 0.1) {
        tileCoordinates.push_back( {startFromLeft, -0.8f} );
        tileCoordinates.push_back( {startFromLeft, 0.8f} );
        if (startFromLeft < -0.3f || startFromLeft > 0.3f) {
            tileCoordinates.push_back( {startFromLeft, 0.0f} );
        }
        if (startFromLeft > -0.4f && startFromLeft < 0.4f) {
            tileCoordinates.push_back( {startFromLeft, 0.4f} );
            tileCoordinates.push_back( {startFromLeft, -0.4f} );
        }
        startFromLeft += 0.1f;
        startFromLeft = roundf(startFromLeft * 100) / 100;
    }
    float startFromTop = 0.8f;
    float endAtBottom = -0.8f;
    while (startFromTop > endAtBottom) {
        tileCoordinates.push_back( {-1.0f, startFromTop} );
        tileCoordinates.push_back( {1.0f, startFromTop} );
        startFromTop -= 0.1f;
        startFromTop = roundf(startFromTop * 100) / 100;
    }
    
    for (int i = 0; i < tileCoordinates.size(); i++) {
        tiles[i].spriteIndex = 1;
        tiles[i].sprite = SheetSprite(appSpritesTexture, tiles[i].spriteIndex, spriteCountX, spriteCountY);
        tiles[i].x = tileCoordinates[i].first;
        tiles[i].y = tileCoordinates[i].second;
        tiles[i].width = 0.1f; tiles[i].height = 0.1f;
        tiles[i].isStatic = true;
        
        entities.push_back(&tiles[i]);
    }

    //COINS
    for (int i = 0; i < MAX_COINS; i++) {
        coins[i].spriteIndex = rand() % 2 + 76;
        coins[i].sprite = SheetSprite(appSpritesTexture, coins[i].spriteIndex, spriteCountX, spriteCountY);
        coins[i].x = (float(rand() % 200) - 99) / 100;
        coins[i].y = (float(rand() % 160) - 79) / 100;
        coins[i].width = 0.1f;
        coins[i].height = 0.1f;
        coins[i].isStatic = false;
        coins[i].enableCollisions = true;
    
        entities.push_back(&coins[i]);
    }
}

void PlatformerApp::Render() {
    GLclampf red = 94/255.0;
    GLclampf green = 128/255.0;
    GLclampf blue = 160/255.0;
    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->Render();
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void PlatformerApp::Update(float elapsed) {
    SDL_Event event; //should this go here OR in App's data members?
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat) { //mid-air jumping
                plr.velocity_y += 3.0f;
            }
            if ( (event.key.keysym.scancode == SDL_SCANCODE_UP || event.key.keysym.scancode == SDL_SCANCODE_W) &&
                !event.key.repeat && plr.collidedBottom == true) { //standard jumping
                plr.velocity_y += 5.0f;
            }
        }
    }
    
    //how to make keys have the same precedence?
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) {
        plr.acceleration_x = 5.0f;
    }
    else if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) {
        plr.acceleration_x = -5.0f;
    }
    else {
        plr.acceleration_x = 0.0f;
    }
    
    for (int i = 0; i < MAX_COINS; i++) {
        if (plr.CollidesWith(&coins[i])) {
            coins[i].x = (float(rand() % 200) - 99) / 100;
            coins[i].y = (float(rand() % 160) - 79) / 100;
        }
    }

}

void PlatformerApp::FixedUpdate() {
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->FixedUpdate(); //error if no include "Entity.h" and timestep definitions also included (no need in App)
        
        entities[i]->collidedTop = false;
        entities[i]->collidedBottom = false;
        entities[i]->collidedLeft = false;
        entities[i]->collidedRight = false;
        
        if (!entities[i]->isStatic) {
            entities[i]->velocity_y += GRAVITY * FIXED_TIMESTEP;
//            entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
//            entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
        }
    }
    
    // handle Y collisiions
    for (int i = 0; i < entities.size(); i++) {
        if (!entities[i]->isStatic && entities[i]->enableCollisions) {
            entities[i]-> y += entities[i]->velocity_y * FIXED_TIMESTEP;
            util.handleCollisionY(entities[i], entities);
        }
    }
    
    // handle X collisions
    for (int i = 0; i < entities.size(); i++) {
        if (!entities[i]->isStatic && entities[i]->enableCollisions) {
            entities[i]-> x += entities[i]->velocity_x * FIXED_TIMESTEP;
            util.handleCollisionX(entities[i], entities);
        }
    }
}

bool PlatformerApp::UpdateAndRender() {
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