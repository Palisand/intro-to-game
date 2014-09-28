//
//  assignment2.cpp
//  This is Pong... Sorta
//
//  Created by Panagis Alisandratos on 9/17/14.
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "entity.h"
using namespace GameObj;

#define PI 3.14159265

SDL_Window* displayWindow;

//global vars of death -- should change!!!
Entity ball, paddle_left, paddle_right;
Entity top_bumper, bottom_bumper, net;
float screen_red = 0.0f;
float screen_blue = 0.0f;
float screen_green = 0.0f;
bool screen_fade = false;
const Uint8* keys = SDL_GetKeyboardState(nullptr);

GLuint LoadTexture(const char* image_path, GLenum image_format = GL_BGRA) {
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

void Setup() {
    SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("This is Pong... Sorta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void SetValues(bool right_won = true) {
    ball.x = 0.0f;
    ball.y = 0.0f;
    ball.dir_x = right_won ? 1.0f : -1.0f;
    ball.dir_y = 0.0f;
    paddle_left.x = -1.0f;
    paddle_left.y = 0.0f;
    paddle_right.x = 1.0f;
    paddle_right.y = 0.0f;
    top_bumper.y = 1.0f;
    top_bumper.x = 0.0f;
    bottom_bumper.y = -1.0f;
    bottom_bumper.x = 0.0f;
}

bool ProcessEvents() {
	SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            return false;
        }
    }
    
    //PADDLE MOVEMENT
    if (keys[SDL_SCANCODE_W])
        paddle_left.dir_y = 1.0f;
    else if (keys[SDL_SCANCODE_S])
        paddle_left.dir_y = -1.0f;
    else
        paddle_left.dir_y = 0.0f;
    
    if (keys[SDL_SCANCODE_UP])
        paddle_right.dir_y = 1.0f;
    else if (keys[SDL_SCANCODE_DOWN])
        paddle_right.dir_y = -1.0f;
    else
        paddle_right.dir_y = 0.0f;
    
    //COLLISIONS
    //ball collide with right paddle side
    if (((ball.x + ball.width * 0.5) > (paddle_right.x - paddle_right.width * 0.5)) &&
        ((ball.y + ball.height * 0.5) > (paddle_right.y - paddle_right.height * 0.5)) &&
        ((ball.y - ball.height * 0.5) < (paddle_right.y + paddle_right.height * 0.5))) {
        ball.x = (paddle_right.x - paddle_right.width * 0.5) - (ball.width * 0.5);
        ball.dir_x = -ball.dir_x;
        float rebound_coeff = (ball.y - paddle_right.y) / (paddle_right.height * 0.5);
        ball.dir_y = rebound_coeff * ball.max_speed;
        screen_red = float(rand() % 10) / 10;
        screen_green = float(rand() % 10) / 10;
        screen_blue = float(rand() % 10) / 10;
    }
    //ball collide with left paddle side
    if (((ball.x - ball.width * 0.5) < (paddle_left.x + paddle_left.width * 0.5)) &&
        ((ball.y + ball.height * 0.5) > (paddle_left.y - paddle_left.height * 0.5)) &&
        ((ball.y - ball.height * 0.5) < (paddle_left.y + paddle_left.height * 0.5))) {
        ball.x = (paddle_left.x + paddle_left.width * 0.5) + (ball.width * 0.5);
        ball.dir_x = -ball.dir_x;
        float rebound_coeff = (ball.y - paddle_left.y) / (paddle_left.height * 0.5);
        ball.dir_y = rebound_coeff * ball.max_speed;
        screen_red = float(rand() % 10) / 10;
        screen_green = float(rand() % 10) / 10;
        screen_blue = float(rand() % 10) / 10;
    }
    //ball collide with top bumper
    if ((ball.y + ball.height * 0.5) > (top_bumper.y - top_bumper.height * 0.5)) {
        ball.y = (top_bumper.y - top_bumper.height * 0.5) - (ball.height * 0.5);
        ball.dir_y = -ball.dir_y;
    }
    //ball collide with bottom bumper
    if ((ball.y - ball.height * 0.5) < (bottom_bumper.y + bottom_bumper.height * 0.5)) {
        ball.y = (bottom_bumper.y + bottom_bumper.height * 0.5) + (ball.height * 0.5);
        ball.dir_y = -ball.dir_y;
    }
    //right paddle collide with top bumper
    if ((paddle_right.y + paddle_right.height * 0.5) > (top_bumper.y - top_bumper.height * 0.5))
        paddle_right.y = (top_bumper.y - top_bumper.height * 0.5) - (paddle_right.height * 0.5);
    //right paddle collide with bottom bumper
    if ((paddle_right.y - paddle_right.height * 0.5) < (bottom_bumper.y + top_bumper.height * 0.5))
        paddle_right.y = (bottom_bumper.y + bottom_bumper.height * 0.5) + (paddle_right.height * 0.5);
    
    //left paddle collide with top bumper
    if ((paddle_left.y + paddle_left.height * 0.5) > (top_bumper.y - top_bumper.height * 0.5))
        paddle_left.y = (top_bumper.y - top_bumper.height * 0.5) - (paddle_left.height * 0.5);
    //left paddle collide with bottom bumper
    if ((paddle_left.y - paddle_left.height * 0.5) < (bottom_bumper.y + top_bumper.height * 0.5))
        paddle_left.y = (bottom_bumper.y + bottom_bumper.height * 0.5) + (paddle_left.height * 0.5);
    
    //SCORING
    if (ball.x > (paddle_right.x + paddle_right.width)) {
        SetValues(false);
        screen_red = 1.0f;
        screen_green = 1.0f;
        screen_blue = 1.0f;
        screen_fade = true;
    }
    if (ball.x < (paddle_left.x - paddle_left.width)) {
        SetValues();
        screen_red = 1.0f;
        screen_green = 1.0f;
        screen_blue = 1.0f;
        screen_fade = true;
    }
    if (screen_fade) {
        screen_red -= 0.02;
        screen_green -= 0.02;
        screen_blue -= 0.02;
        if (screen_red <= 0.0)
            screen_fade = false;
    }
    
    return true;
}

void Update(float elapsed) {
    paddle_left.y += paddle_left.dir_y * elapsed;
    paddle_right.y += paddle_right.dir_y * elapsed;
    ball.x += ball.dir_x * elapsed;
    ball.y += ball.dir_y * elapsed;
    ball.max_speed = 1.5;
}

void Render() {
    glClearColor(screen_red, screen_green, screen_blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ball.SetSize(0.1, 0.1);
    ball.Draw();
    paddle_left.SetSize(0.3, 0.1);
    paddle_left.Draw();
    paddle_right.SetSize(0.3, 0.1);
    paddle_right.Draw();
    
    top_bumper.SetSize(0.1, 2);
    top_bumper.Draw();
    bottom_bumper.SetSize(0.1, 2);
    bottom_bumper.Draw();
    
    net.SetSize(1.5, 0.02);
    net.Draw();
    
    SDL_GL_SwapWindow(displayWindow);
}

void Cleanup() {
    // For texture clean up (will not be used in this assignment)
}

int main(int argc, char *argv[]) {
    Setup();
    
    float lastFrameTicks = 0.0f;
    
    SetValues();
    
	while (ProcessEvents()) {
        
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        Update(elapsed);
        
        Render();
    }
    
	SDL_Quit();
	return 0;
}