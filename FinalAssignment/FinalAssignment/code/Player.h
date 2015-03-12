//
//  Player.h
//  FinalAssignment
//

#ifndef __FinalAssignment__Player__
#define __FinalAssignment__Player__

#define MAX_BULLETS 2
#define BULLET_SPEED 200
#define KICKBACK_SPEED 100
#define JUMP_SPEED 4.5f
#define SPAWN_DELAY 100

#include "Entity.h"
#include "Projectile.h"
#include "Head.h"
#include "Flag.h"
#include "Sound.h"
#include <vector>

class Player : public Entity {
public:
    
    Player();
    
    void Update(float elapsed);
    void SetAnims(int animStart, int animLength, float animSpeed);
    void BuildHead();
    void BuildBullets();
    void BuildMine();
    void Shoot();
    void PlaceMine();
    void Jump();
    void InitiateDeathSequence(bool dropHead = false, Flag* flag = nullptr);
    void Respawn();
    
    int animStartOrig;
    int animEndOrig;
    //bool isDead; // cannot get key input is unplayble... isVisible will do just fine
    bool canDoubleJump;
    bool canShoot;
    bool wasAirborne;
    int ammoCount;
    int bulletIndex;
    Projectile bullets[MAX_BULLETS];
    Head head;
    ParticleEmitter emitter;
    Entity mine;
    bool holdingFlag;
    int mineIndex;
    float sinInc;
    float spawnDelay;
    std::vector<std::pair<float, float>> spawnPoints;
    int score;
    float scoreTextSize;
};

#endif /* defined(__FinalAssignment__Player__) */
