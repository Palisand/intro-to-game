//
//  Application.cpp
//  FinalAssignment
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Application.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;


Application::Application() {
    Init();
    done = false;
    lastFrameTicks = 0.0f;
    timeLeftOver = 0.0f;
    keys = SDL_GetKeyboardState(nullptr);
    state = STATE_TITLE_SCREEN;
    
    currMenuSel = 0;
    menuItemSizes.assign(TITLE_MENU_ITEM_COUNT, 0.05f);
    
    bgR = 1.0f;
    bgG = 1.0f;
    bgB = 1.0f;
    bgA = 1.0f;
    
    scale = SCALE;
    translateX = -1.6f;
    translateY = 1.2f;
    
    perlinVal = 0;
    
    xMove1 = 0;
    xMove2 = 0;
    
//    SDL_JoystickEventState(SDL_ENABLE);
    plr1Controller = SDL_JoystickOpen(0);
//    plr1Haptic = SDL_HapticOpenFromJoystick(plr1Controller);
//    SDL_HapticRumbleInit(plr1Haptic);
    
    plr2Controller = SDL_JoystickOpen(1);
    
//    SDL_HapticEffect effect;
//    memset(&effect, 0, sizeof(SDL_HapticEffect));
//    shootRumble.type = SDL_HAPTIC_CONSTANT;
//    shootRumble.periodic.period = 500;
//    shootRumble.periodic.magnitude = 2000;
//    shootRumbleId = SDL_HapticNewEffect(plr1Haptic, &shootRumble);
    
//    effect.type = SDL_HAPTIC_RAMP;
//    effect.ramp.length = 1000;
//    effect.ramp.start = 10000;
//    effect.ramp.end = 0;
    
//    effect.type = SDL_HAPTIC_CONSTANT;
//    effect.constant.length = 1000;
//    effect.constant.level = 32767;
    
//    shootRumbleId = SDL_HapticNewEffect(plr1Haptic, &effect);
    
    BuildGlobalAssets();
//    BuildTitleScreen();
}


Application::~Application() {
    
    SDL_JoystickClose( plr1Controller );
//    SDL_HapticDestroyEffect(plr1Haptic, shootRumbleId);
//    SDL_HapticClose( plr1Haptic );
    plr1Controller = nullptr;
//    plr1Haptic = nullptr;
    SDL_JoystickClose( plr2Controller );
    
    for (Sound& sound : soundEffects) {
        sound.Quit();
    }
    Mix_FreeMusic(music);
    
    SDL_Quit();
}


void Application::Init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
    displayWindow = SDL_CreateWindow("This Game Right Here", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1200, 900, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 1200, 900);
    glMatrixMode(GL_PROJECTION);
//    glOrtho(-GL_ORTHO_X, GL_ORTHO_X, -GL_ORTHO_Y, GL_ORTHO_Y, -1.0, 1.0); //change
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
}


void Application::BuildGlobalAssets() {
    
    spriteSheetTexId = util.LoadTexture("assets/img/spritesheet.png");
    sheetCountX = 16;
    sheetCountY = 11;
    
    fontSheetTexId = util.LoadTexture("assets/img/fontsheet.png");
    p1r = 1.0f; p1g = 1.0f; p1b = 1.0f;
    p2r = 1.0f; p2g = 1.0f; p2b = 1.0f;
    
    //sounds are pre-loaded
    for (int i = 1; i < 6; i++) {
        soundEffects.push_back("assets/sound/effects/bump" + to_string(i) + ".wav");
    }
    for (int i = 1; i < 4; i++) {
        soundEffects.push_back("assets/sound/effects/bob" + to_string(i) + ".wav");
    }
    soundEffects.push_back(Sound("assets/sound/effects/flag_score.wav"));
    
    string musicPath = "assets/sound/music/";
    songs.push_back(musicPath + "50centwar.mp3");
    songs.push_back(musicPath + "ascension.mp3");
    songs.push_back(musicPath + "ctech.mp3");
    songs.push_back(musicPath + "pop.mp3");
    songs.push_back(musicPath + "rockdub.mp3");
    songs.push_back(musicPath + "rowan.mp3");
    songs.push_back(musicPath + "seth_theme.mp3");
    songs.push_back(musicPath + "weird.mp3");
    
    music = Mix_LoadMUS("assets/sound/music/gameover.mp3");
    Mix_VolumeMusic(30);
    Mix_PlayMusic(music, -1);
}


void Application::BuildHeadHunterLevel() {
    music = Mix_LoadMUS(songs[util.randRange(0, 7)].c_str());
    Mix_VolumeMusic(30);
    Mix_PlayMusic(music, -1);
    ResetGlobalEntities();
    BuildMap("assets/tileData.txt", TYPE_HEAD_HUNTER);
    SpawnGlobalEntities();
}


void Application::BuildCaptureTheFlagLevel() {
    
    music = Mix_LoadMUS(songs[util.randRange(0, 7)].c_str());
    Mix_VolumeMusic(30);
    Mix_PlayMusic(music, -1);
    
    ResetGlobalEntities();
    
    plr1Flag.SetAnims(sheetCountX * 7, 9, 0.4f);
    plr1Flag.sprite = SheetSprite(spriteSheetTexId, plr1Flag.animStart, sheetCountX, sheetCountY);
    plr1Flag.spawnPoint.animStart = 138;
    plr1Flag.spawnPoint.animEnd = 140;
    plr1Flag.spawnPoint.sprite = SheetSprite(spriteSheetTexId, plr1Flag.spawnPoint.animStart, sheetCountX, sheetCountY);
    entities.push_back(&plr1Flag);
    entities.push_back(&plr1Flag.spawnPoint);
    plr2Flag.SetAnims(sheetCountX * 8, 9, 0.4f);
    plr2Flag.sprite = SheetSprite(spriteSheetTexId, plr2Flag.animStart, sheetCountX, sheetCountY);
    plr2Flag.spawnPoint.animStart = 139;
    plr2Flag.spawnPoint.animEnd = 140;
    plr2Flag.spawnPoint.sprite = SheetSprite(spriteSheetTexId, plr2Flag.spawnPoint.animStart, sheetCountX, sheetCountY);
    entities.push_back(&plr2Flag);
    entities.push_back(&plr2Flag.spawnPoint);
    
    BuildMap("assets/tileData.txt", TYPE_CAPTURE_THE_FLAG);
    plr1Flag.Return();
    plr2Flag.Return();
    SpawnGlobalEntities();
}


void Application::BuildKingOfTheHillLevel() {
    
    music = Mix_LoadMUS("assets/sound/music/koth_pop.mp3");
    Mix_VolumeMusic(30);
    Mix_PlayMusic(music, -1);
    
    ResetGlobalEntities();
    
    theHill.sprite = SheetSprite(spriteSheetTexId, sheetCountX * 9 + 12, sheetCountX, sheetCountY);
    theHill.width = 0.1f * 2;
    theHill.height = 0.1f;
    theHill.hasGrav = false;
    theHill.isStatic = true;
    theHill.isVisible = true;
    theHill.enableCollisions = false;
    
    KOTH_barScale = 0;
    timer = 14700;
    
    BuildMap("assets/tileData.txt", TYPE_KING_OF_THE_HILL);
    SpawnGlobalEntities();
}


void Application::ResetGlobalEntities() {
    
    //PLAYER 1
    plr1.SetAnims(0, 5, 0.8f);
    plr1.sprite = SheetSprite(spriteSheetTexId, plr1.animStart, sheetCountX, sheetCountY);
    plr1.score = 0;
    plr1.ammoCount = 0;
    plr1.width = 0.1f;
    plr1.height = 0.1f;
    plr1.hasGrav = true;
    plr1.isStatic = false;
    plr1.isVisible = true;
    plr1.enableCollisions = true;
    plr1.velocity_x = 0.0f;
    plr1.velocity_y = 0.0f;
    plr1.acceleration_x = 0.0f;
    plr1.acceleration_y = 0.0f;
    plr1.friction_x = 7.0f;
    plr1.friction_y = 4.0f;
    
    plr1.head.SetAnims(sheetCountX * 4, 23, 0.4f);
    plr1.head.sprite = SheetSprite(spriteSheetTexId, plr1.head.animStart, sheetCountX, sheetCountY);
    entities.push_back(&plr1.head);
    
    entities.push_back(&plr1);
    emitters.push_back(&plr1.emitter);

    for (int i = 0; i < MAX_BULLETS; i++) {
        plr1.bullets[i].muzzleFlashIndex = sheetCountX - 2;
        plr1.bullets[i].sprite = SheetSprite(spriteSheetTexId, sheetCountX - 1, sheetCountX, sheetCountY);
        entities.push_back(&plr1.bullets[i]);
        emitters.push_back(&plr1.bullets[i].emitter);
    }
    
    plr1.mine.sprite = SheetSprite(spriteSheetTexId, sheetCountX * 9, sheetCountX, sheetCountY);
    plr1.mineIndex = sheetCountX * 9 + 1;
    entities.push_back(&plr1.mine);
    
    plr1Ammo.SetAnims(sheetCountX * 2, sheetCountX - 1, 0.4f);
    plr1Ammo.sprite = SheetSprite(spriteSheetTexId, plr1Ammo.animStart, sheetCountX, sheetCountY);
    plr1Ammo.width = 0.1f;
    plr1Ammo.height= 0.1f;
    plr1Ammo.hasGrav = false;
    plr1Ammo.isStatic = true;
    plr1Ammo.isVisible = true;
    plr1Ammo.enableCollisions = false;
    entities.push_back(&plr1Ammo);
    
    
    //PLAYER 2
    plr2.SetAnims(sheetCountX, 5, 0.8f);
    plr2.sprite = SheetSprite(spriteSheetTexId, plr2.animStart, sheetCountX, sheetCountY);
    plr2.score = 0;
    plr2.ammoCount = 0;
    plr2.width = 0.1f;
    plr2.height = 0.1f;
    plr2.hasGrav = true;
    plr2.isStatic = false;
    plr2.isVisible = true;
    plr2.enableCollisions = true;
    plr2.velocity_x = 0.0f;
    plr2.velocity_y = 0.0f;
    plr2.acceleration_x = 0.0f;
    plr2.acceleration_y = 0.0f;
    plr2.friction_x = 7.0f;
    plr2.friction_y = 4.0f;
    
    plr2.head.SetAnims(sheetCountX * 5 + 8, 23, 0.4f);
    plr2.head.sprite = SheetSprite(spriteSheetTexId, plr2.head.animStart, sheetCountX, sheetCountY);
    entities.push_back(&plr2.head);
    
    entities.push_back(&plr2);
    emitters.push_back(&plr2.emitter);
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        plr2.bullets[i].muzzleFlashIndex = sheetCountX * 2 - 2;
        plr2.bullets[i].sprite = SheetSprite(spriteSheetTexId, sheetCountX * 2 - 1, sheetCountX, sheetCountY);
        entities.push_back(&plr2.bullets[i]);
        emitters.push_back(&plr2.bullets[i].emitter);
    }
    
    plr2.mine.sprite = SheetSprite(spriteSheetTexId, sheetCountX * 10, sheetCountX, sheetCountY);
    plr2.mineIndex = sheetCountX * 10 + 1;
    entities.push_back(&plr2.mine);
    
    plr2Ammo.SetAnims(sheetCountX * 3, sheetCountX - 1, 0.4f);
    plr2Ammo.sprite = SheetSprite(spriteSheetTexId, plr2Ammo.animStart, sheetCountX, sheetCountY);
    plr2Ammo.width = 0.1f;
    plr2Ammo.height= 0.1f;
    plr2Ammo.hasGrav = false;
    plr2Ammo.isStatic = true;
    plr2Ammo.isVisible = true;
    plr2Ammo.enableCollisions = false;
    entities.push_back(&plr2Ammo);
}


void Application::BuildMap(const string& tileMapPath, int type) {

    ifstream infile(tileMapPath);
    string line;
    
    while (getline(infile, line)) {
        if (line == "[header]") {
            if (!readHeader(infile)) {
                return;
            }
        }
//        else if (line == "[layer]") {
//            readLayerData(infile, "Tile Layer 1");
//        }
//        else {
//            switch (type) {
//                case TYPE_HEAD_HUNTER:
//                    if (line == "[Object Layer 1H]") {
//                        //number based on tile layer number, H,C,B based on type
//                        readEntityData(infile);
//                    }
//                    break;
//                case TYPE_CAPTURE_THE_FLAG:
//                    if (line == "[Object Layer 1C]") {
//                        //number based on tile layer number, H,C,B based on type
//                        readEntityData(infile);
//                    }
//                    break;
//                case TYPE_KING_OF_THE_HILL:
//                    if (line == "[Object Layer 1B]") {
//                        //number based on tile layer number, H,C,B based on type
//                        readEntityData(infile);
//                    }
//                    break;
//            }
//        }
        
        else {
            switch (1) { //(int)util.randRange(1, 3)
                case 1:
                    if (line == "[layer]") {
                        readLayerData(infile, "Tile Layer 1"); //better names than numbers
                    }
                    else {
                        switch (type) {
                            case TYPE_HEAD_HUNTER:
                                if (line == "[Object Layer 1H]") {
                                    readEntityData(infile);
                                }
                                break;
                            case TYPE_CAPTURE_THE_FLAG:
                                if (line == "[Object Layer 1C]") {
                                    readEntityData(infile);
                                }
                                break;
                            case TYPE_KING_OF_THE_HILL:
                                if (line == "[Object Layer 1K]") {
                                    readEntityData(infile);
                                }
                                break;
                        }
                    }
                    break;
                case 2:
                    if (line == "[layer]") {
                        readLayerData(infile, "Tile Layer 2"); //better names than numbers
                    }
                    else {
                        switch (type) {
                            case TYPE_HEAD_HUNTER:
                                if (line == "[Object Layer 2H]") {
                                    readEntityData(infile);
                                }
                                break;
                            case TYPE_CAPTURE_THE_FLAG:
                                if (line == "[Object Layer 2C]") {
                                    readEntityData(infile);
                                }
                                break;
                            case TYPE_KING_OF_THE_HILL:
                                if (line == "[Object Layer 2K]") {
                                    readEntityData(infile);
                                }
                                break;
                        }
                    }
                    break;
                case 3:
                    if (line == "[layer]") {
                        readLayerData(infile, "Tile Layer 3"); //better names than numbers
                    }
                    else {
                        switch (type) {
                            case TYPE_HEAD_HUNTER:
                                if (line == "[Object Layer 3H]") {
                                    readEntityData(infile);
                                }
                                break;
                            case TYPE_CAPTURE_THE_FLAG:
                                if (line == "[Object Layer 3C]") {
                                    readEntityData(infile);
                                }
                                break;
                            case TYPE_KING_OF_THE_HILL:
                                if (line == "[Object Layer 3K]") {
                                    readEntityData(infile);
                                }
                                break;
                        }
                    }
                    break;
            }
        }
    }
    
}


void Application::SpawnGlobalEntities() {
    plr1.Respawn();
    plr1Ammo.Respawn();
    plr2.Respawn();
    plr2Ammo.Respawn();
}


void Application::Render() {
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch (state) {
        case STATE_TITLE_SCREEN:
            RenderTitleScreen();
            break;
        case STATE_INSTRUCTIONS_SCREEN:
            RenderInstructions();
            break;
        case STATE_HEAD_HUNTER_LEVEL:
            RenderHeadHunterLevel();
            break;
        case STATE_CAPTURE_THE_FLAG_LEVEL:
            RenderCaptureTheFlagLevel();
            break;
        case STATE_KING_OF_THE_HILL_LEVEL:
            RenderKingOfTheHillLevel();
            break;
        case STATE_GAME_OVER:
            RenderGameOver();
    }
    
    SDL_GL_SwapWindow(displayWindow);
}


void Application::RenderTitleScreen() {
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    util.DrawText(fontSheetTexId, "LET'S SHOOT EACHOTHER", 0.08f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.7f, util.ALIGN_CENTER);
    
    util.DrawText(fontSheetTexId, "Head Hunter", menuItemSizes[0], -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.3f, util.ALIGN_CENTER);
    util.DrawText(fontSheetTexId, "Capture The Flag", menuItemSizes[1], -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.2f, util.ALIGN_CENTER);
    util.DrawText(fontSheetTexId, "King Of The Hill", menuItemSizes[2], -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.1f, util.ALIGN_CENTER);
    
    util.DrawText(fontSheetTexId, "Instructions", menuItemSizes[3], -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -0.1f, util.ALIGN_CENTER);

    util.DrawText(fontSheetTexId, "Modifiers", menuItemSizes[4], -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -0.2f, util.ALIGN_CENTER);
    
    util.DrawText(fontSheetTexId, "Quit", menuItemSizes[5], -0.01f, 1.0f, 1.0f, 1.0f, 1.0, 0.0f, -0.3f, util.ALIGN_CENTER);
    
    if (notReady) {
        util.DrawText(fontSheetTexId, "Coming Soon", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -0.9f, util.ALIGN_CENTER);
    }
    
}


void Application::RenderInstructions() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    util.DrawText(fontSheetTexId, "Press A to jump and A in the air to double jump", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.8f, util.ALIGN_CENTER);
    util.DrawText(fontSheetTexId, "Press B to drop or re-place a mine", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.6f, util.ALIGN_CENTER);
    util.DrawText(fontSheetTexId, "Move to your colored bullet to load your gun", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.4f, util.ALIGN_CENTER);
    util.DrawText(fontSheetTexId, "Shoot by pressing Right Button / Trigger when your gun is loaded", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.2f, util.ALIGN_CENTER);
    util.DrawText(fontSheetTexId, "You can only hold 1 bullet at a time", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, util.ALIGN_CENTER);
    
    util.DrawText(fontSheetTexId, "Head Hunter - Collect heads after kills", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -0.3f, util.ALIGN_CENTER);
    util.DrawText(fontSheetTexId, "Capture the Flag - Bring enemy flag to your base", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -0.5f, util.ALIGN_CENTER);
    util.DrawText(fontSheetTexId, "King of the Hill - Be the king until time expires", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -0.7f, util.ALIGN_CENTER);
    
    util.DrawText(fontSheetTexId, "Press [Back] to return to the title menu...", 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -0.9f, util.ALIGN_CENTER);
}


void Application::RenderCamera() {
    if (fabs(plr1.x - plr2.x) < 50) {
        if (plr1.x > plr2.x) {
            centerX = -plr1.x + ( (plr1.x - plr2.x) / 2 );
        }
        else {
            centerX = -plr2.x + ( (plr2.x - plr1.x) / 2 );
        }
        
        if (plr1.y > plr2.y) {
            centerY = -plr1.y + ( (plr1.y - plr2.y) / 2 );
        }
        else {
            centerY = -plr2.y + ( (plr2.y - plr1.y) / 2 );
        }
    }
    else {
        centerX = -mapWidth * TILE_SIZE / 2;
        centerY = mapHeight * TILE_SIZE / 2;
    }
    
    glScalef(scale, scale, 0.0f);
    glTranslatef(translateX, translateY, 0.0f);
    glTranslatef(sin(screenShakeValX * screenShakeSpeed) * screenShakeIntensity, 0.0f, 0.0f);
    glTranslatef(noise1(perlinVal), noise1(perlinVal + 100.0f), 0.0);
}


void Application::RenderHeadHunterLevel() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    RenderCamera();

    RenderBackground();
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->Render();
    }
    RenderLevel();
    for (size_t i = 0; i < emitters.size(); i++) {
        emitters[i]->Render();
    }
    
    //render level hud
    util.DrawText(fontSheetTexId, "PLAYER 1", 0.05f, -0.01f, p1r, p1g, p1b, 1.0f, -1.25f, 0.9f);
    util.DrawText(fontSheetTexId, to_string(plr1.score), plr1.scoreTextSize, -0.01f, p1r, p1g, p1b, 1.0f, -1.25f, 0.8f);
    util.DrawText(fontSheetTexId, "PLAYER 2", 0.05f, -0.01f, p2r, p2g, p2b, 1.0f, 1.25f, 0.9f, util.ALIGN_RIGHT);
    util.DrawText(fontSheetTexId, to_string(plr2.score), plr2.scoreTextSize, -0.01f, p2r, p2g, p2b, 1.0f, 1.25f, 0.8f, util.ALIGN_RIGHT);
}


void Application::RenderCaptureTheFlagLevel() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    RenderCamera();
    
    RenderBackground();
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->Render();
    }
    RenderLevel();
    for (size_t i = 0; i < emitters.size(); i++) {
        emitters[i]->Render();
    }
    
    //render level hud
    util.DrawText(fontSheetTexId, "PLAYER 1", 0.05f, -0.01f, p1r, p1g, p1b, 1.0f, -1.25f, 0.9f);
    util.DrawText(fontSheetTexId, to_string(plr1.score), plr1.scoreTextSize, -0.01f, p1r, p1g, p1b, 1.0f, -1.25f, 0.8f);
    util.DrawText(fontSheetTexId, "PLAYER 2", 0.05f, -0.01f, p2r, p2g, p2b, 1.0f, 1.25f, 0.9f, util.ALIGN_RIGHT);
    util.DrawText(fontSheetTexId, to_string(plr2.score), plr2.scoreTextSize, -0.01f, p2r, p2g, p2b, 1.0f, 1.25f, 0.8f, util.ALIGN_RIGHT);
    
    util.DrawText(fontSheetTexId, plr1FlagStatus, 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, -1.25f, -0.9f);
    util.DrawText(fontSheetTexId, plr2FlagStatus, 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 1.25f, -0.9f, util.ALIGN_RIGHT);
    
    if (plr1.holdingFlag) {
        util.DrawSprite(spriteSheetTexId, 0.05f, 0.05f, 1.0f, 1.0f, -0.9f, 0.9f, sheetCountX * 8, sheetCountX, sheetCountY);
    }
    if (plr2.holdingFlag) {
        util.DrawSprite(spriteSheetTexId, 0.05f, 0.05f, 1.0f, 1.0f, 0.87f, 0.9f, sheetCountX * 7, sheetCountX, sheetCountY);
    }
}


void Application::RenderKingOfTheHillLevel() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    RenderCamera();
    
    RenderBackground();
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->Render();
    }
    RenderLevel();
    theHill.Render();
    for (size_t i = 0; i < emitters.size(); i++) {
        emitters[i]->Render();
    }
    
    //render level hud
    util.DrawText(fontSheetTexId, "PLAYER 1", 0.05f, -0.01f, p1r, p1g, p1b, 1.0f, -1.25f, 0.9f);
    util.DrawText(fontSheetTexId, "PLAYER 2", 0.05f, -0.01f, p2r, p2g, p2b, 1.0f, 1.25f, 0.9f, util.ALIGN_RIGHT);
    
    util.DrawText(fontSheetTexId, "TIME LEFT: " + to_string(timer), 0.05f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -0.8f, util.ALIGN_CENTER);
    util.DrawSprite(0, 0.05f, 0.02f, KOTH_barScale, 1.0f, 0.0f, -0.9f, 0, sheetCountX, sheetCountY, true);
}


void Application::RenderGameOver() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    util.DrawText(fontSheetTexId, "ENOUGH BLOODSHED", 0.08f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f, util.ALIGN_CENTER);
    
    if (plr1.score > plr2.score) {
        util.DrawText(fontSheetTexId, "RED WINS", 0.05f, -0.01f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, util.ALIGN_CENTER);
    }
    else if (plr2.score > plr1.score) {
        util.DrawText(fontSheetTexId, "BLUE WINS", 0.05f, -0.01f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, util.ALIGN_CENTER);
    }
    else {
        util.DrawText(fontSheetTexId, "TIE, EVERYONE LOSES", 0.05f, -0.01f, 0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f, util.ALIGN_CENTER);
    }
    
}


void Application::RenderBackground() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(mapWidth * TILE_SIZE / 2, -mapHeight * TILE_SIZE / 2, 0.0f);
    
    GLfloat quad[] = {
        -mapWidth * TILE_SIZE / 2, mapHeight * TILE_SIZE / 2,
        -mapWidth * TILE_SIZE / 2, -mapHeight * TILE_SIZE / 2,
        mapWidth * TILE_SIZE / 2, -mapHeight * TILE_SIZE / 2,
        mapWidth * TILE_SIZE / 2, mapHeight * TILE_SIZE / 2,
    };
    
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat quadColors[] = {
        bgR, bgG, bgB, bgA,
        bgR, bgG, bgB, bgA,
        bgR, bgG, bgB, bgA,
        bgR, bgG, bgB, bgA,
    };
    
    glColorPointer(4, GL_FLOAT, 0, quadColors);
    glEnableClientState(GL_COLOR_ARRAY);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY); //see what happens if you do not disable

    glPopMatrix();
}


void Application::RenderLevel() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, spriteSheetTexId);
    
    vector<float> vertexData;
    vector<float> texCoordData;
    
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            float u = (float)(((int)levelData[y][x] - 1) % sheetCountX) / (float)sheetCountX; // - 1 to account for Tiled's index strangeness
            float v = (float)(((int)levelData[y][x] - 1) / sheetCountX) / (float)sheetCountY;
            float width = 1.0f / (float)sheetCountX;
            float height = 1.0f / (float)sheetCountY;
            
            if (levelData[y][x] != 0) {
                vertexData.insert(vertexData.end(), {
                    TILE_SIZE * x,                  -TILE_SIZE * y,
                    TILE_SIZE * x,                  (-TILE_SIZE * y) - TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE,    (-TILE_SIZE * y) - TILE_SIZE,
                    (TILE_SIZE * x) + TILE_SIZE,    -TILE_SIZE * y
                });
                
                texCoordData.insert(texCoordData.end(), {
                    u + UV_OFFSET,          v + UV_OFFSET,
                    u + UV_OFFSET,          v + height - UV_OFFSET,
                    u + width - UV_OFFSET,  v + height - UV_OFFSET,
                    u + width - UV_OFFSET,  v + UV_OFFSET
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
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glPopMatrix();
}


void Application::Update(float elapsed) {
    
    switch (state) {
        case STATE_TITLE_SCREEN:
            UpdateTitleScreen(elapsed);
            break;
        case STATE_INSTRUCTIONS_SCREEN:
            UpdateInstructions(elapsed);
            break;
        case STATE_HEAD_HUNTER_LEVEL:
            UpdateHeadHunterLevel(elapsed);
            break;
        case STATE_CAPTURE_THE_FLAG_LEVEL:
            UpdateCaptureTheFlagLevel(elapsed);
            break;
        case STATE_KING_OF_THE_HILL_LEVEL:
            UpdateKingOfTheHillLevel(elapsed);
            break;
        case STATE_GAME_OVER:
            UpdateGameOver(elapsed);
    }

}


void Application::UpdateTitleScreen(float elapsed) {
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_DOWN:
                    //move down menu items
                    ++currMenuSel;
                    if (currMenuSel > TITLE_MENU_ITEM_COUNT - 1) {
                        currMenuSel = 0;
                    }
                    break;
                case SDL_SCANCODE_UP:
                    //move up menu items
                    --currMenuSel;
                    if (currMenuSel < 0) {
                        currMenuSel = TITLE_MENU_ITEM_COUNT - 1;
                    }
                    break;
                case SDL_SCANCODE_RETURN:
                    switch (currMenuSel) {
                        case 0:
                            BuildHeadHunterLevel();
                            state = STATE_HEAD_HUNTER_LEVEL;
                            break;
                        case 1:
                            BuildCaptureTheFlagLevel();
                            state = STATE_CAPTURE_THE_FLAG_LEVEL;
                            break;
                        case 2:
                            BuildKingOfTheHillLevel();
                            state = STATE_KING_OF_THE_HILL_LEVEL;
                            break;
                        case 3:
                            state = STATE_INSTRUCTIONS_SCREEN;
                            break;
                        case 5: //Quit
                            done = true;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        else if (event.type == SDL_JOYBUTTONDOWN) {
            
            if (event.jbutton.which == 0) {
                
                if (event.jbutton.button == 11) {
                    --currMenuSel;
                    if (currMenuSel < 0) {
                        currMenuSel = TITLE_MENU_ITEM_COUNT - 1;
                    }
                }
                if (event.jbutton.button == 12) {
                    ++currMenuSel;
                    if (currMenuSel > TITLE_MENU_ITEM_COUNT - 1) {
                        currMenuSel = 0;
                    }
                }
                if (event.jbutton.button == 0) {
                    switch (currMenuSel) {
                        case 0:
                            BuildHeadHunterLevel();
                            state = STATE_HEAD_HUNTER_LEVEL;
                            break;
                        case 1:
                            BuildCaptureTheFlagLevel();
                            state = STATE_CAPTURE_THE_FLAG_LEVEL;
                            break;
                        case 2:
                            BuildKingOfTheHillLevel();
                            state = STATE_KING_OF_THE_HILL_LEVEL;
                            break;
                        case 3:
                            break;
                        case 5: //Quit
                            done = true;
                        default:
                            break;
                    }
                }
            }
        }
    }
    
    if (currMenuSel == 4) { //modifiers
        notReady = true;
    }
    else {
        notReady = false;
    }
    
    for (size_t i = 0; i < TITLE_MENU_ITEM_COUNT; i++) {
        if (i != currMenuSel) {
            if (menuItemSizes[i] > 0.05f) {
                menuItemSizes[i] -= 0.01f;
            }
            else {
                menuItemSizes[i] = 0.05f;
            }
        }
    }
    
    if (menuItemSizes[currMenuSel] < 0.1f) {
        menuItemSizes[currMenuSel] += 0.01f;
    }
    else {
        menuItemSizes[currMenuSel] = 0.1f;
    }
}


void Application::UpdateInstructions(float elapsed) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                state = STATE_TITLE_SCREEN;
            }
        }
        else if (event.type == SDL_JOYBUTTONDOWN) {
            if (event.jbutton.button == 9) {
                state = STATE_TITLE_SCREEN;
            }
        }
    }
}


void Application::UpdateHeadHunterLevel(float elapsed) {

    UpdateInput();
    
    UpdateCamera(elapsed);
    
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->Update(elapsed);
    }
    
    for (int i = 0; i < emitters.size(); i++) {
        emitters[i]->Update(elapsed);
    }
    
    //COLLISIONS
    PlrOnPlr();
    PlrOnAmmo();
    PlrOnHead();
    PlrOnMine(true, false);
    PlrOnBullet(true, false);
    BulletOnBullet();
    
    if (plr1.score > 100 || plr2.score > 100) {
        music = Mix_LoadMUS("assets/sound/music/gameover.mp3");
        Mix_VolumeMusic(30);
        Mix_PlayMusic(music, -1);
        state = STATE_GAME_OVER;
    }
}


void Application::UpdateCaptureTheFlagLevel(float elapsed) {
    
    UpdateInput();
    
    UpdateCamera(elapsed);
    
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->Update(elapsed);
    }
    
    for (int i = 0; i < emitters.size(); i++) {
        emitters[i]->Update(elapsed);
    }
    
    //COLLISIONS
    PlrOnPlr();
    PlrOnAmmo();
    PlrOnFlag();
    PlrOnFlagSpawn();
    PlrOnMine(false, true);
    PlrOnBullet(false, true);
    BulletOnBullet();
    
    if (plr1.score > 500 || plr2.score > 500) {
        music = Mix_LoadMUS("assets/sound/music/gameover.mp3");
        Mix_VolumeMusic(30);
        Mix_PlayMusic(music, -1);
        state = STATE_GAME_OVER;
    }
}


void Application::UpdateKingOfTheHillLevel(float elapsed) {
    
    UpdateInput();
    
    UpdateCamera(elapsed);
    
    for (int i = 0; i < entities.size(); i++) {
        entities[i]->Update(elapsed);
    }
    
    for (int i = 0; i < emitters.size(); i++) {
        emitters[i]->Update(elapsed);
    }
    
    if (KOTH_barScale > 0) {
        plr1.score = fabs(KOTH_barScale);
        plr2.score = 0;
    }
    else if (KOTH_barScale < 0) {
        plr2.score = fabs(KOTH_barScale);
        plr1.score = 0;
    }
    else {
        plr1.score = 0;
        plr2.score = 0;
    }
    
    if (timer > 0) {
        timer -= elapsed * 100;
    }
    else {
        music = Mix_LoadMUS("assets/sound/music/gameover.mp3");
        Mix_VolumeMusic(30);
        Mix_PlayMusic(music, -1);
        state = STATE_GAME_OVER;
    }
    
    //COLLISIONS
    PlrOnPlr();
    PlrOnAmmo();
    PlrOnHill();
    PlrOnMine(false, false);
    PlrOnBullet(false, false);
    BulletOnBullet();
}


void Application::UpdateGameOver(float elapsed) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                state = STATE_TITLE_SCREEN;
            }
        }
        else if (event.type == SDL_JOYBUTTONDOWN) {
            if (event.jbutton.button == 0) {
                state = STATE_TITLE_SCREEN;
            }
        }
    }
    
    entities.erase(entities.begin(), entities.end());
    emitters.erase(emitters.begin(), emitters.end());
}


void Application::UpdateInput() {
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                music = Mix_LoadMUS("assets/sound/music/gameover.mp3");
                Mix_VolumeMusic(30);
                Mix_PlayMusic(music, -1);
                entities.erase(entities.begin(), entities.end());
                emitters.erase(emitters.begin(), emitters.end());
                state = STATE_TITLE_SCREEN;
            }
            if (!event.key.repeat) { //USE SWITCH
                if (plr1.isVisible) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_W) {
                        plr1.Jump();
                    }
                    if ((event.key.keysym.scancode == SDL_SCANCODE_D || event.key.keysym.scancode == SDL_SCANCODE_A) && plr1.collidedBottom) {
                        plr1.sprite.index = plr1.animStart + 1;
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT) {
                        plr1.Shoot();
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_S && !plr1.mine.isVisible) {
                        plr1.PlaceMine();
                    }
                }
                if (plr2.isVisible) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                        plr2.Jump();
                    }
                    if ((event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_LEFT) && plr1.collidedBottom) {
                        plr2.sprite.index = plr2.animStart + 1;
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_RSHIFT) {
                        plr2.Shoot();
                    }
                    
                    if (event.key.keysym.scancode == SDL_SCANCODE_DOWN && !plr2.mine.isVisible) {
                        plr2.PlaceMine();
                    }
                }
            }
        }
        else if (event.type == SDL_JOYAXISMOTION) {
            //            cout << (int)event.jaxis.axis << endl;
            switch (event.jaxis.axis) {
                case 0:
                    switch (event.jaxis.which) {
                        case 0:
                            xMove1 = ((float)event.jaxis.value / 32767.0f);
                            break;
                        case 1:
                            xMove2 = ((float)event.jaxis.value / 32767.0f);
                            break;
                        default:
                            break;
                    }
                    break;
                case 5:
                    switch (event.jaxis.which) {
                        case 0:
                            if (plr1.isVisible) {
                                plr1.Shoot();
                            }
                            break;
                        case 1:
                            if (plr2.isVisible) {
                                plr2.Shoot();
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                    
            }
            //            if (event.jaxis.axis == 0) {
            //                switch (event.jaxis.which) {
            //                    case 0:
            //                        xMove1 = ((float)event.jaxis.value / 32767.0f);
            //                        break;
            //                    case 1:
            //                        xMove2 = ((float)event.jaxis.value / 32767.0f);
            //                        break;
            //                    default:
            //                        break;
            //                }
            //            }
            
        }
        else if (event.type == SDL_JOYBUTTONDOWN) {
//                        cout << (int)event.jbutton.button << endl;
            if (event.jbutton.button == 9) {
                music = Mix_LoadMUS("assets/sound/music/gameover.mp3");
                Mix_VolumeMusic(30);
                Mix_PlayMusic(music, -1);
                entities.erase(entities.begin(), entities.end());
                emitters.erase(emitters.begin(), emitters.end());
                state = STATE_TITLE_SCREEN;
            }
            switch (event.jbutton.which) {
                case 0:
                    HandlePlayerButtonAction(event, plr1);
                    break;
                case 1:
                    HandlePlayerButtonAction(event, plr2);
                    break;
                default:
                    break;
            }
        }
    }
    
    if (plr1.isVisible) {
        //PLAYER 1 - JOY INPUT
        if (fabs(xMove1) > 0.4) {
            plr1.acceleration_x = (xMove1 > 0) ? 10.0f : -10.0f;
        }
        //PLAYER 1 - KEY INPUT
        else if (keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A]) {
            plr1.acceleration_x = 10.0f;
        }
        else if (keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) {
            plr1.acceleration_x = -10.0f;
        }
        else {
            plr1.acceleration_x = 0.0f;
        }
    }
    
    if (plr2.isVisible) {
        //PLAYER 2 - JOY INPUT
        if (fabs(xMove2) > 0.4) {
            plr2.acceleration_x = (xMove2 > 0) ? 10.0f : -10.0f;
        }
        //PLAYER 2 - KEY INPUT
        else if (keys[SDL_SCANCODE_RIGHT] && !keys[SDL_SCANCODE_LEFT]) {
            plr2.acceleration_x = 10.0f;
        }
        else if (keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT]) {
            plr2.acceleration_x = -10.0f;
        }
        else {
            plr2.acceleration_x = 0.0f;
        }
    }
}


void Application::HandlePlayerButtonAction(SDL_Event& event, Player& plr) {
    switch (event.jbutton.button) {
        case 0:
            plr.Jump();
            break;
        case 5:
            plr.Shoot();
//            SDL_HapticRunEffect(plr1Haptic, shootRumbleId, 1);
            break;
        case 1:
            plr.PlaceMine();
            break;
    }
}


void Application::UpdateCamera(float elapsed) {
    
    if (fabs(plr1.x - plr2.x) < 0.5 && fabs(plr1.y - plr2.y) < 0.5) {
        if (scale < MAX_SCALE) {
            scale += 0.05;
            bgR = 1; bgG = 1; bgB = 1;
        }
    }
    else {
        if (scale > SCALE) {
            scale -= 0.05;
        }
    }
    
    if (translateX > centerX + 0.01) {
        translateX -= 0.01;
    }
    else if (translateX < centerX - 0.01) {
        translateX += 0.01;
    }
    else {
        translateX = centerX;
    }
    
    if (translateY > centerY + 0.01) {
        translateY -= 0.01;
    }
    else if (translateY < centerY - 0.01) {
        translateY += 0.01;
    }
    else {
        translateY = centerY;
    }
    
    //SCREEN EFFECTS
    if (shakeTime > 0) {
        screenShakeValX += elapsed * shakeTime;
        shakeTime--;
    }
    else {
        shakeTime = 0;
    }
    
    if (fabs(plr1.x - plr2.x) < 0.5 && fabs(plr1.y - plr2.y) < 0.5 && plr1.isVisible && plr2.isVisible) {
        perlinVal += elapsed / 10;
        screenShakeValX += elapsed;
//        bgG = sin(foo * PI / 180);
//        bgB = sin(foo * PI / 180);
        bgR = fabs(sin(foo * PI / 180)) / 1.5 + 0.2;
        bgG = fabs(sin(foo * PI / 180)) / 1.5 + 0.2;
        bgB = fabs(sin(foo * PI / 180)) / 1.5 + 0.2;
        foo += 10;
    }
    else {
        if (bgFlashTime > 0) {
            perlinVal += elapsed * perlinDeathMult;
            bgR = sin(foo/10 * PI / 180);
            bgG = sin(foo/2 * PI / 180);
            bgB = sin(foo * PI / 180);
            bgFlashTime--;
            foo += 100;
        }
        else {
            perlinVal += elapsed / 10;
            bgR = fabs(sin(foo * PI / 180)) / 4.5 + 0.2;
            bgG = fabs(sin(foo * PI / 180)) / 4.5 + 0.2;
            bgB = fabs(sin(foo * PI / 180)) / 4.5 + 0.2;
            perlinDeathMult = 25;
            bgFlashTime = 0;
            foo += 1;
        }
    }
}


void Application::FixedUpdate() {
    
    for (size_t i = 0; i < entities.size(); i++) {
        entities[i]->FixedUpdate();
        
        entities[i]->collidedTop = false;
        entities[i]->collidedBottom = false;
        entities[i]->collidedLeft = false;
        entities[i]->collidedRight = false;
        
        if (entities[i]->hasGrav) {
            entities[i]->velocity_y += GRAVITY * FIXED_TIMESTEP;
        }
    }
    
    // handle Y collisions
    for (int i = 0; i < entities.size(); i++) {
        if (!entities[i]->isStatic) {
            entities[i]-> y += entities[i]->velocity_y * FIXED_TIMESTEP;
            if (entities[i]->enableCollisions) {
                handleCollisionY(entities[i]);
            }
        }
    }
    
    // handle X collisions
    for (int i = 0; i < entities.size(); i++) {
        if (!entities[i]->isStatic) {
            entities[i]-> x += entities[i]->velocity_x * FIXED_TIMESTEP;
            if (entities[i]->enableCollisions) {
                handleCollisionX(entities[i]);
            }
        }
    }
}


bool Application::UpdateAndRender() {
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


bool Application::readHeader(ifstream& stream) {
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
        else if (key == "tilewidth") {
            tileWidth = atoi(value.c_str());
        }
        else if (key == "tileheight") {
            tileHeight = atoi(value.c_str());
        }
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


bool Application::readLayerData(std::ifstream& stream, const string& tileLayer) {
    string line;
    
    while (getline(stream, line)) {
        if (line == "") {
            break;
        }
        
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        
        if (value == tileLayer) {
            getline(stream, line); //skip 'data=' line
//        }
//        if (key == "data") {
            for (int y = 0; y < mapHeight; y++) { //get rows
                getline(stream, line);
                istringstream lineStream(line);
                string tile;
                
                for (int x = 0; x < mapWidth; x++) { //get row values
                    getline(lineStream, tile, ',');
                    unsigned char val = (unsigned char)atoi(tile.c_str());
                    if (val > 0) {
                        //flare map file tiles are indexed from 1 (not 0)
                        if (val == 126) { //126 = crate sprite tile
                            switch (rand() % 2) {
                                case 0:
                                    levelData[y][x] = util.randRange(val, val + 3);
                                    break;
                                case 1:
                                    levelData[y][x] = util.randRange(val + 16, val + 16 + 3);
                                    break;
                            }
                        }
                        else {
                            levelData[y][x] = val;
                        }
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


bool Application::readEntityData(std::ifstream& stream) {
    
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
            
            float placeX = atoi(xPosition.c_str()) / tileWidth * TILE_SIZE + (TILE_SIZE / 2);
            float placeY = atoi(yPosition.c_str()) / tileHeight * -TILE_SIZE + (TILE_SIZE / 2);
            
            storeLocation(type, placeX, placeY);
        }
    }
    return true;
}


void Application::storeLocation(string type, float x, float y) {
    if (type == "RedSpawnPoint") {
        plr1.spawnPoints.push_back(make_pair(x, y));
    }
    if (type == "BlueSpawnPoint") {
        plr2.spawnPoints.push_back(make_pair(x, y));
    }
    if (type == "AmmoSpawnPoint") {
        plr1Ammo.spawnPoints.push_back(make_pair(x, y));
        plr2Ammo.spawnPoints.push_back(make_pair(x, y));
    }
    if (type == "FlagSpawnPoint1") {
        plr1Flag.spawnPoint.x = x;
        plr1Flag.spawnPoint.y = y;
    }
    if (type == "FlagSpawnPoint2") {
        plr2Flag.spawnPoint.x = x;
        plr2Flag.spawnPoint.y = y;
    }
    if (type == "CapturePoint") {
        theHill.x = x;
        theHill.y = y;
    }
}


bool Application::isSolid(unsigned char tile) {
    switch (tile) {
        case 158:
        case 159:
        case 160:
        case 171:
        case 172:
        case 173:
        case 174:
        case 175:
        case 176:
            return true;
            break;
        default:
            return false;
            break;
    }
}


void Application::PlrOnPlr() {
    if (plr1.CollidesWith(&plr2) && plr1.isVisible && plr2.isVisible) {
        soundEffects[util.randRange(0, 5)].Play(1);
        if (plr1.x < plr2.x) {
            plr1.velocity_x = -2.0f;
            plr2.velocity_x = 2.0f;
        }
        else if (plr2.x < plr1.x) {
            plr1.velocity_x = 2.0f;
            plr2.velocity_x = -2.0f;
        }
        
        if (plr1.y > plr2.y) {
            plr1.velocity_y = 2.0f;
        }
        else if (plr2.y > plr1.y) {
            plr2.velocity_y = 2.0f;
        }
    }
}


void Application::PlrOnAmmo() {
    if (plr1.CollidesWith(&plr1Ammo) && plr1.isVisible && plr1.ammoCount == 0) {
        plr1.score += MIN_SCORE;
        plr1.scoreTextSize *= 3;
        plr1Ammo.PickUp();
        plr1.ammoCount++;
    }
    if (plr2.CollidesWith(&plr2Ammo) && plr2.isVisible && plr2.ammoCount == 0) {
        plr2.score += MIN_SCORE;
        plr2.scoreTextSize *= 3;
        plr2Ammo.PickUp();
        plr2.ammoCount++;
    }
    
    if (plr1.ammoCount == 0) {
        p1r = 1.0f; p1g = 1.0f; p1b = 1.0f;
    }
    else {
        p1g = 0; p1b = 0;
    }
    if (plr2.ammoCount == 0) {
        p2r = 1.0f; p2g = 1.0f; p2b = 1.0f;
    }
    else {
        p2r = 0; p2g = 0;
    }
    
    plr1Ammo.inUse.first = plr2Ammo.x;
    plr1Ammo.inUse.second = plr2Ammo.y;
    plr2Ammo.inUse.first = plr1Ammo.x;
    plr2Ammo.inUse.second = plr1Ammo.y;
}


void Application::PlrOnHead() {
    if (plr1.CollidesWith(&plr2.head) && plr1.isVisible && plr2.head.isVisible) {
        plr1.score += COLLECT_SCORE;
        plr1.scoreTextSize *= 3;
        plr2.head.PickUp();
    }
    if (plr2.CollidesWith(&plr1.head) && plr2.isVisible && plr1.head.isVisible) {
        plr2.score += COLLECT_SCORE;
        plr2.scoreTextSize *= 3;
        plr1.head.PickUp();
    }
}


void Application::PlrOnHill() {
    if (plr1.CollidesWith(&theHill, 0.0f, 0.1f) && plr1.isVisible) {
        theHill.sprite.index = sheetCountX * 9 + 2;
        KOTH_barScale += 0.01;
    }
    else if (plr2.CollidesWith(&theHill, 0.0f, 0.1f) && plr2.isVisible) {
        theHill.sprite.index = sheetCountX * 10 + 2;
        KOTH_barScale -= 0.01;
    }
    else {
        theHill.sprite.index = sheetCountX * 9 + 12;
    }
    
    if (plr1.CollidesWith(&theHill, 0.0f, 0.1f) && plr2.CollidesWith(&theHill, 0.0f, 0.1f) && plr1.isVisible && plr2.isVisible) {
        theHill.sprite.index = sheetCountX * 9 + 12;
        KOTH_barScale -= 0.01; //counteract above
    }
}


void Application::PlrOnFlag() {
    if (plr1.CollidesWith(&plr2Flag) && plr1.isVisible && plr2Flag.isVisible) {
        plr1.score += COLLECT_SCORE * 2;
        plr1.scoreTextSize *= 3;
        plr2Flag.PickUp();
        plr2FlagStatus = "FLAG TAKEN";
        plr1.holdingFlag = true;
    }
    if (plr1.CollidesWith(&plr1Flag) && plr1.isVisible && plr1Flag.isVisible) {
        plr1Flag.Return();
        plr1FlagStatus = "";
    }
    
    if (plr2.CollidesWith(&plr1Flag) && plr2.isVisible && plr1Flag.isVisible) {
        plr2.score += COLLECT_SCORE * 2;
        plr2.scoreTextSize *= 3;
        plr1Flag.PickUp();
        plr1FlagStatus = "FLAG TAKEN";
        plr2.holdingFlag = true;
    }
    if (plr2.CollidesWith(&plr2Flag) && plr2.isVisible && plr2Flag.isVisible) {
        plr2Flag.Return();
        plr2FlagStatus = "";
    }
}


void Application::PlrOnFlagSpawn() {
    if (plr1.CollidesWith(&plr1Flag.spawnPoint) && plr1.holdingFlag) {
        plr1.score += COLLECT_SCORE * 5;
        plr1.scoreTextSize *= 3;
        plr2Flag.Return();
        plr2FlagStatus = "";
        plr1.holdingFlag = false;
        soundEffects[8].Play(1);
    }
    if (plr2.CollidesWith(&plr2Flag.spawnPoint) && plr2.holdingFlag) {
        plr2.score += COLLECT_SCORE * 5;
        plr2.scoreTextSize *= 3;
        plr1Flag.Return();
        plr1FlagStatus = "";
        plr2.holdingFlag = false;
        soundEffects[8].Play(1);
    }
}


void Application::PlrOnMine(bool dropHead, bool dropFlag) {
    if (plr1.CollidesWithCenter(&plr2.mine) && plr2.mine.isVisible) {
        bgFlashTime = 30;
        plr2.score += SHOOT_SCORE;
        plr2.scoreTextSize *= 3;
        if (dropFlag) {
            plr1.InitiateDeathSequence(dropHead, &plr2Flag);
        }
        else {
            plr1.InitiateDeathSequence(dropHead);
        }
        plr2.mine.isVisible = false;
    }
    if (plr2.CollidesWithCenter(&plr1.mine) && plr1.mine.isVisible) {
        bgFlashTime = 30;
        plr1.score += SHOOT_SCORE;
        plr1.scoreTextSize *= 3;
        if (dropFlag) {
            plr2.InitiateDeathSequence(dropHead, &plr1Flag);
        }
        else {
            plr2.InitiateDeathSequence(dropHead);
        }
        plr1.mine.isVisible = false;
    }
}


void Application::PlrOnBullet(bool dropHead, bool dropFlag) {
    for (Projectile& bullet : plr1.bullets) {
        if (plr2.CollidesWith(&bullet) && plr2.isVisible) {
            //bullet.impact (and emit)
            if (bullet.isVisible) {
                bgFlashTime = 30;
                bullet.emitter.EmitX(10, (bullet.sprite.x_scale > 0) ? true : false);
                plr1.score += SHOOT_SCORE * fabs(plr1.x - plr2.x) + MIN_SCORE;
                plr1.scoreTextSize *= 3;
                if (dropFlag) {
                    plr2.InitiateDeathSequence(dropHead, &plr1Flag);
                }
                else {
                    plr2.InitiateDeathSequence(dropHead);
                }
                bullet.isVisible = false;
            }
        }
    }
    for (Projectile& bullet : plr2.bullets) {
        if (plr1.CollidesWith(&bullet) && plr1.isVisible) {
            //bullet.impact
            if (bullet.isVisible) {
                bgFlashTime = 30;
                bullet.emitter.EmitX(10, (bullet.sprite.x_scale > 0) ? true : false);
                plr2.score += SHOOT_SCORE * fabs(plr1.x - plr2.x) + MIN_SCORE;
                plr2.scoreTextSize *= 3;
                if (dropFlag) {
                    plr1.InitiateDeathSequence(dropHead, &plr2Flag);
                }
                else {
                    plr1.InitiateDeathSequence(dropHead);
                }
                bullet.isVisible = false;
            }
        }
    }
}


void Application::BulletOnBullet() {
    for (Projectile& b1 : plr1.bullets) {
        for (Projectile& b2 : plr2.bullets) {
            if (b1.CollidesWithCenter(&b2) && b1.isVisible && b2.isVisible) {
                b1.emitter.EmitX(10, (b1.sprite.x_scale > 0) ? true : false);
                b2.emitter.EmitX(10, (b2.sprite.x_scale > 0) ? true : false);
                soundEffects[util.randRange(5, 8)].Play(2);
                shakeTime = 30;
                b1.isVisible = false;
                b2.isVisible = false;
            }
        }
    }
}


void Application::worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY) {
    *gridX = (int)(worldX / TILE_SIZE);
    *gridY = (int)(-worldY / TILE_SIZE);
}


float Application::checkPointCollisionX(float x, float y) {
    int gridX, gridY;
    worldToTileCoordinates(x, y, &gridX, &gridY);
    
    if (gridX < 0 || gridX > mapWidth - 1 || gridY < 0 || gridY > mapHeight - 1) {
        return 0.0f;
    }
    
    if (isSolid(levelData[gridY][gridX])) {
        float xCoordinate = (gridX + 1) * TILE_SIZE;
        return xCoordinate - x;
    }
    
    return 0.0f;
}


float Application::checkPointCollisionY(float x, float y) {
    int gridX, gridY;
    worldToTileCoordinates(x, y, &gridX, &gridY);
    
    if (gridX < 0 || gridX > mapWidth - 1 || gridY < 0 || gridY > mapHeight - 1) {
        return 0.0f;
    }
    
    if (isSolid(levelData[gridY][gridX])) {
        float yCoordinate = gridY * TILE_SIZE;
        return -y - yCoordinate;
    }
    
    return 0.0f;
}


void Application::handleCollisionX(Entity* entity) {
    
    //left
    float penetration = checkPointCollisionX(entity->x - (entity->width * entity->x_scale * 0.5), entity->y);
    if (penetration != 0.0f) {
        entity->x += penetration / COLLISION_DAMP;
        entity->velocity_x = 0.0f;
        entity->collidedLeft = true;
    }
    
    //right
    penetration = checkPointCollisionX(entity->x + (entity->width * entity->x_scale * 0.5), entity->y);
    if (penetration != 0.0f) {
        entity->x += (penetration - TILE_SIZE) / COLLISION_DAMP;
        entity->velocity_x = 0.0f;
        entity->collidedRight = true;
    }
}


void Application::handleCollisionY(Entity* entity) {
    
//    //bottom
//    float penetration = checkPointCollisionY(entity->x, entity->y - (entity->height * entity->y_scale * 0.5));
//    if (penetration != 0.0f) {
//        entity->y += penetration;
//        entity->velocity_y = 0.0f;
//        entity->collidedBottom = true;
//    }
//    
//    //top
//    penetration = checkPointCollisionY(entity->x, entity->y + (entity->height * entity->y_scale * 0.5));
//    if (penetration != 0.0f) {
//        entity->y += (penetration - TILE_SIZE);
//        entity->velocity_y = 0.0f;
//        entity->collidedTop = true;
//    }
    
    //bottom right
    float penetration = checkPointCollisionY(entity->x + (entity->width * 0.3), entity->y - (entity->height * entity->y_scale * 0.5));
    if (penetration != 0.0f) {
        entity->y += penetration;
        entity->velocity_y = 0.0f;
        entity->collidedBottom = true;
    }
    //bottom left
    penetration = checkPointCollisionY(entity->x - (entity->width * 0.3), entity->y - (entity->height * entity->y_scale * 0.5));
    if (penetration != 0.0f) {
        entity->y += penetration;
        entity->velocity_y = 0.0f;
        entity->collidedBottom = true;
    }
    //top right
    penetration = checkPointCollisionY(entity->x + (entity->width * 0.3), entity->y + (entity->height * entity->y_scale * 0.5));
    if (penetration != 0.0f) {
        entity->y += (penetration - TILE_SIZE);
        entity->velocity_y = 0.0f;
        entity->collidedTop = true;
    }
    //top left
    penetration = checkPointCollisionY(entity->x - (entity->width * 0.3), entity->y + (entity->height * entity->y_scale * 0.5));
    if (penetration != 0.0f) {
        entity->y += (penetration - TILE_SIZE);
        entity->velocity_y = 0.0f;
        entity->collidedTop = true;
    }
}