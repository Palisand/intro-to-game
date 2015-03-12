//
//  Player.cpp
//  FinalAssignment
//

#include "Player.h"
#include "Entity.h"
#include <cmath>

Player::Player() {
    canShoot = false;
    ammoCount = 0;
    bulletIndex = 0;
    BuildHead();
    BuildBullets();
    BuildMine();
    emitter = ParticleEmitter(100);
    score = 0;
    scoreTextSize = 0.05f;
    holdingFlag = false;
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    for (int i = 1; i < 6; i++) {
        soundEffects.push_back("assets/sound/effects/jump" + std::to_string(i) + ".wav");
        soundEffects[i-1].SetVolume(10);
    }
    
    for (int i = 1; i < 4; i++) {
        soundEffects.push_back("assets/sound/effects/shoot" + std::to_string(i) + ".wav");
    }
    
    soundEffects.push_back(Sound("assets/sound/effects/land.wav"));
    soundEffects[8].SetVolume(50);
    
    for (int i = 1; i < 7; i++) {
        soundEffects.push_back("assets/sound/effects/explosion" + std::to_string(i) + ".wav");
    }
    
    for (int i = 1; i < 6; i++) {
        soundEffects.push_back("assets/sound/effects/double_jump" + std::to_string(i) + ".wav");
        soundEffects[i+14].SetVolume(50);
    }
    
    for (int i = 1; i < 6; i++) {
        soundEffects.push_back("assets/sound/effects/place_mine" + std::to_string(i) + ".wav");
    }
}

void Player::Update(float elapsed) {
    
    //handle double jump
    if (collidedBottom) {
        canDoubleJump = true;
    }
    
    //handle spawns
    if (spawnDelay > -1) {
        spawnDelay--;
    }
    if (spawnDelay == 0) {
        Respawn();
    }
    
    //handle shooting
    if (ammoCount > 0) {
        canShoot = true;
    }
    else {
        canShoot = false;
    }
    //handle canShoot indicator (color gun)
    if (canShoot) {
        animStart = animStartOrig + animLength + 1;
        animEnd = animEndOrig + animLength + 1;
    }
    else {
        animStart = animStartOrig;
        animEnd = animEndOrig;
    }
    
    if (fabs(acceleration_x)) {
        if (collidedBottom) {
            sprite.index += animSpeed;
            if (sprite.index > animEnd + 1) {
                sprite.index = animStart;
            }
        }
        else {
            sprite.index = animStart + 1;
        }
    
        if ((sprite.x_scale < 0 && acceleration_x > 0) ||
            (sprite.x_scale > 0 && acceleration_x < 0)){
            sprite.FlipX();
        }
    }
    else {
        if (collidedBottom) {
            sprite.index = animStart;
        }
        else {
            sprite.index = animStart + 1;
        }
    }
    
    if (fabs(velocity_y) > 0) {
        wasAirborne = true;
    }
    
    if (collidedBottom && velocity_y == 0 && wasAirborne) {
        soundEffects[8].Play(1); //same as jump channel
        wasAirborne = false;
    }
    
    //get your own head back
    if (CollidesWith(&head) && isVisible && head.isVisible) {
        head.PickUp();
    }
    
    emitter.position.x = x;
    emitter.position.y = y;
    
    //mine anim - don't feel like creating some tiny class for this guy
    mine.sprite.index = sin(sinInc += 0.5f) + mineIndex;
    
//    scoreTextSize = util.mapValue(fabs(velocity_x), 0.0, 1.0, 0.05, 0.05*3);
    
    if (scoreTextSize > 0.05f) {
        scoreTextSize -= 0.01f;
    }
    else {
        scoreTextSize = 0.05;
    }
}


void Player::SetAnims(int animStart, int animLength, float animSpeed) {
    this->animStart = animStart;
    animEnd = animStart + animLength;
    animStartOrig = animStart;
    animEndOrig = animEnd;
    this->animLength = animLength;
    this->animSpeed = animSpeed;
}


void Player::BuildHead() {
    
    head.width = 0.07f;
    head.height = 0.05f;
    head.hasGrav = true;
    head.isStatic = false;
    head.isVisible = false;
    head.enableCollisions = true;
    head.velocity_x = 0.0f;
    head.velocity_y = 0.0f;
    head.acceleration_x = 0.0f;
    head.acceleration_y = 0.0f;
    head.friction_x = 7.0f;
    head.friction_y = 4.0f;
    
}


void Player::BuildBullets() {
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].width = 0.1f;
        bullets[i].height = 0.1f;
        bullets[i].hasGrav = false;
        bullets[i].isStatic = false;
        bullets[i].enableCollisions = true;
        bullets[i].isVisible = false;
        bullets[i].velocity_x = 0;
        bullets[i].velocity_y = 0;
        bullets[i].friction_x = 0;
        bullets[i].friction_y = 0;
        bullets[i].acceleration_x = 0;
        bullets[i].acceleration_y = 0;
    }
    
}

void Player::BuildMine() {
    
    mine.width = 0.1f;
    mine.height = 0.1f;
    mine.hasGrav = false;
    mine.isStatic = true;
    mine.isVisible = false;
    mine.enableCollisions = true;
    
}


void Player::Shoot() {
    
    if (canShoot) {
        
        soundEffects[util.randRange(5, 8)].Play(2);
        
        ammoCount--;
        
        bullets[bulletIndex].isVisible = true;
        bullets[bulletIndex].sprite.index--;
        
        bullets[bulletIndex].x = x;
        bullets[bulletIndex].y = y;
        if (sprite.x_scale > 0) {
            bullets[bulletIndex].velocity_x = BULLET_SPEED * FIXED_TIMESTEP;
            if (bullets[bulletIndex].sprite.x_scale < 0) {
                bullets[bulletIndex].sprite.FlipX();
            }
            
            velocity_x = -KICKBACK_SPEED * FIXED_TIMESTEP;
        }
        else {
            bullets[bulletIndex].velocity_x = -BULLET_SPEED * FIXED_TIMESTEP;
            if (bullets[bulletIndex].sprite.x_scale > 0) {
                bullets[bulletIndex].sprite.FlipX();
            }
            
            velocity_x = KICKBACK_SPEED * FIXED_TIMESTEP;
        }
        bullets[bulletIndex].velocity_y = 0;
        
        bulletIndex++;
        if (bulletIndex > MAX_BULLETS -1) {
            bulletIndex = 0;
        }
        
    }
    
}

void Player::PlaceMine() {
    if (collidedBottom) {
        soundEffects[util.randRange(20,25)].Play(-1);
        mine.x = x;
        mine.y = y;
        mine.isVisible = true;
    }
}

void Player::Jump() {
    if (collidedBottom) {
        soundEffects[util.randRange(0,5)].Play(1);
        velocity_y = JUMP_SPEED;
    }
    if (!collidedBottom && canDoubleJump) {
        emitter.EmitYoffsetX(5, false, (sprite.x_scale > 0) ? false : true, width/2);
        soundEffects[util.randRange(15,20)].Play(1);
        velocity_y = JUMP_SPEED - 1;
        canDoubleJump = false;
    }
}


void Player::InitiateDeathSequence(bool dropHead, Flag* flag) {
    
    emitter.particleDecayRate = 0.2f;
    emitter.velocityDeviation.y = 3.0f;
    emitter.EmitY(100, false); //change to centered emission and ADD COLOR!
    //restore defaults
    emitter.particleDecayRate = 1.0f;
    emitter.velocityDeviation.y = 1.0f;
    
    mine.isVisible = false;
    
    if (dropHead) {
        head.Spawn(x, y);
    }
    
    if (flag && holdingFlag) {
        flag->Drop(x, y);
        holdingFlag = false;
    }
    
    soundEffects[util.randRange(9, 15)].Play(2);
    spawnDelay = SPAWN_DELAY;
    isVisible = false;
    ammoCount = 0;
    //place out of map
//    x = 99;
    
}


void Player::Respawn() {
    
    int coordinates = util.randRange(0, spawnPoints.size() - 1);
    
    x = spawnPoints[coordinates].first;
    y = spawnPoints[coordinates].second;
    
    isVisible = true;
    
}