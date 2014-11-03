//
//  AsteroidsApp.cpp
//  Assigment7
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "AsteroidsApp.h"
#include <iostream>
using namespace std;

AsteroidsApp::AsteroidsApp() {
    Init();
    done = false;
    lastFrameTicks = 0.0f;
    keys = SDL_GetKeyboardState(nullptr);
    
    BuildLevel();
}

AsteroidsApp::~AsteroidsApp() {
    SDL_Quit();
}

void AsteroidsApp::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Extremely Minimalistic Asteroids Purely for the Implementation of Matrices and the SAT", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void AsteroidsApp::BuildLevel() {
    //PLAYER
    GLuint plrSpriteTexture = 0;
    plr.sprite = SheetSprite(plrSpriteTexture, 1, 1, 1);
    plr.x = 0.0f;
    plr.y = 0.0f;
    plr.scale_x = 1.0f;
    plr.scale_y = 1.0f;
    plr.rotation = 0.0f;
    plr.width = 0.1f;
    plr.height = 0.1f;
    plr.isStatic = false;
    plr.enableCollisions = true;
    plr.velocity_x = 0.0f;
    plr.velocity_y = 0.0f;
    plr.velocity_r = 0.0f;
    plr.acceleration_x = 0.0f;
    plr.acceleration_y = 0.0f;
    plr.acceleration_r = 0.0f;
    plr.friction_x = 3.0f;
    plr.friction_y = 3.0f;
    plr.friction_r = 3.0f;
    entities.push_back(&plr);

    //ENEMIES
    GLuint enemySpriteTexture = 0;
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroids[i].sprite = SheetSprite(enemySpriteTexture, 1, 1, 1);
        asteroids[i].x = util.randRange(-1.33f, 1.33f);
        asteroids[i].y = util.randRange(-1.0f, 1.0f);
        asteroids[i].scale_x = 1.0f;
        asteroids[i].scale_y = 1.0f;
        asteroids[i].rotation = util.randRange(0, 360);
        asteroids[i].width = util.randRange(0.3f, 0.5f);
        asteroids[i].height = util.randRange(0.3f, 0.5f);
        asteroids[i].isStatic = false;
        asteroids[i].enableCollisions = true;
        asteroids[i].velocity_x = 0.0f;
        asteroids[i].velocity_y = 0.0f;
        asteroids[i].velocity_r = 0.0f;
        asteroids[i].acceleration_x = -sin(util.randRange(0, 360) * PI / 180.0f) * util.randRange(1.0f, 2.0f);
        asteroids[i].acceleration_y = cos(util.randRange(0, 360) * PI / 180.0f) * util.randRange(1.0f, 2.0f);
        asteroids[i].acceleration_r = util.randRange(-300, 300);
        asteroids[i].friction_x = 3.0f;
        asteroids[i].friction_y = 3.0f;
        asteroids[i].friction_r = 3.0f;
        entities.push_back(&asteroids[i]);
    }
    
}

void AsteroidsApp::Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);

    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->Render();
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void AsteroidsApp::Update(float elapsed) {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
    }
    
    if (keys[SDL_SCANCODE_W]) {
        plr.acceleration_x = -sin(plr.rotation * PI / 180.0f) * 3.0f;
        plr.acceleration_y = cos(plr.rotation * PI / 180.0f) * 3.0f;
    }
    else {
        plr.acceleration_x = 0.0f;
        plr.acceleration_y = 0.0f;
    }
    
    if (keys[SDL_SCANCODE_A]) {
        plr.acceleration_r = 600.0f;
    }
    else if (keys[SDL_SCANCODE_D]) {
        plr.acceleration_r = -600.0f;
    }
    else {
        plr.acceleration_r = 0.0f;
    }
}

void AsteroidsApp::FixedUpdate() {
    
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->FixedUpdate();
        
        for (size_t j = 0; j < entities.size(); j++) {
            if (CheckCollision(entities[i], entities[j])) {
                
                Vector diff(entities[i]->x - entities[j]->x, entities[i]->y - entities[j]->y, 0.0f);
                diff.normalize();

                entities[i]->x += diff.x / 100;
                entities[i]->y += diff.y / 100;
            }
        }
        
        //screen-wrap
        
        if (entities[i] == &plr) {
            if (entities[i]->x > 1.33) {
                entities[i]->x = -1.33;
            }
            if (entities[i]->x < -1.33) {
                entities[i]->x = 1.33;
            }
            if (entities[i]->y > 1.0) {
                entities[i]->y = -1.0;
            }
            if (entities[i]->y < -1.0) {
                entities[i]->y = 1.0;
            }
        }
        else {
            if (entities[i]->x > 1.80) {
                entities[i]->x = -1.5;
            }
            if (entities[i]->x < -1.80) {
                entities[i]->x = 1.5;
            }
            if (entities[i]->y > 1.50) {
                entities[i]->y = -1.3;
            }
            if (entities[i]->y < -1.50) {
                entities[i]->y = 1.3;
            }
        }
    }

}

bool AsteroidsApp::UpdateAndRender() {
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

bool AsteroidsApp::CheckCollision(Entity* entity1, Entity* entity2) {

    if (entity1 == entity2) { //entities cannot collide with themselves
        return false;
    }
    
    //build each entities matrix
    entity1->buildMatrix();
    entity2->buildMatrix();
    
    //create their inverses
    Matrix entity1Inverse = entity1->matrix.inverse();
    Matrix entity2Inverse = entity2->matrix.inverse();
    
    //Entity2 - - - - - - - - - - - - - - - - - - - - -
    
    //get edge point vectors (without any translations, rotations, or scalings accounted for)
    Vector ent2TL(-entity2->width * 0.5, entity2->height * 0.5, 0.0); //top left
    Vector ent2TR(entity2->width * 0.5, entity2->height * 0.5, 0.0); //top right
    Vector ent2BL(-entity2->width * 0.5, -entity2->height * 0.5, 0.0); // bottom left
    Vector ent2BR(entity2->width * 0.5, -entity2->height * 0.5, 0.0); // bottom right
    
    //transfrom to world coordinates
    //multiply edge point vectors by matrix
    ent2TL = entity2->matrix * ent2TL; //why no *= ? because of overload?
    ent2TR = entity2->matrix * ent2TR;
    ent2BL = entity2->matrix * ent2BL;
    ent2BR = entity2->matrix * ent2BR;
    
    //express edge point vectors in terms of entity's object space
    //multiply edge point vectors by inverse matrix of the other entity
    ent2TL = entity1Inverse * ent2TL;
    ent2TR = entity1Inverse * ent2TR;
    ent2BL = entity1Inverse * ent2BL;
    ent2BR = entity1Inverse * ent2BR;
    
    //get smallest and largest X values
    float minX = fmin( fmin( fmin(ent2TL.x, ent2TR.x), ent2BL.x), ent2BR.x);
    float maxX = fmax( fmax( fmax(ent2TL.x, ent2TR.x), ent2BL.x), ent2BR.x);
    
    //check x collision
    if (!(minX <= entity1->width * 0.5 && maxX >= -entity1->width * 0.5)) {
        return false;
    }
    
    //get smallest and largest Y values
    float minY = fmin( fmin( fmin(ent2TL.y, ent2TR.y), ent2BL.y), ent2BR.y);
    float maxY = fmax( fmax( fmax(ent2TL.y, ent2TR.y), ent2BL.y), ent2BR.y);
    
    //check y collision
    if (!(minY <= entity1->height * 0.5 && maxY >= -entity1->height * 0.5)) {
        return false;
    }
    
    //Entity2 - - - - - - - - - - - - - - - - - - - - -
    
    //get edge point vectors (without any translations, rotations, or scalings accounted for)
    Vector ent1TL(-entity1->width * 0.5, entity1->height * 0.5, 0.0); //top left
    Vector ent1TR(entity1->width * 0.5, entity1->height * 0.5, 0.0); //top right
    Vector ent1BL(-entity1->width * 0.5, -entity1->height * 0.5, 0.0); // bottom left
    Vector ent1BR(entity1->width * 0.5, -entity1->height * 0.5, 0.0); // bottom right
    
    //transfrom to world coordinates
    //multiply edge point vectors by matrix
    ent1TL = entity1->matrix * ent1TL;
    ent1TR = entity1->matrix * ent1TR;
    ent1BL = entity1->matrix * ent1BL;
    ent1BR = entity1->matrix * ent1BR;
    
    //express edge point vectors in terms of entity's object space
    //multiply edge point vectors by inverse matrix of the other entity
    ent1TL = entity2Inverse * ent1TL;
    ent1TR = entity2Inverse * ent1TR;
    ent1BL = entity2Inverse * ent1BL;
    ent1BR = entity2Inverse * ent1BR;
    
    //get smallest and largest X values
    minX = fmin( fmin( fmin(ent1TL.x, ent1TR.x), ent1BL.x), ent1BR.x);
    maxX = fmax( fmax( fmax(ent1TL.x, ent1TR.x), ent1BL.x), ent1BR.x);
    
    //check x collision
    if (!(minX <= entity2->width * 0.5 && maxX >= -entity2->width * 0.5)) {
        return false;
    }
    
    //get smallest and largest Y values
    minY = fmin( fmin( fmin(ent1TL.y, ent1TR.y), ent1BL.y), ent1BR.y);
    maxY = fmax( fmax( fmax(ent1TL.y, ent1TR.y), ent1BL.y), ent1BR.y);
    
    //check y collision
    if (!(minY <= entity2->height * 0.5 && maxY >= -entity2->height * 0.5)) {
        return false;
    }
    
    return true;
}
