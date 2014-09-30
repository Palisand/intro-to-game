//
//  assignment3.cpp
//  Space Invaders: Ms. Pacman Edition
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "SpaceInvadersApp.h"

int main(int argc, char *argv[])
{
    SpaceInvadersApp app;
    while(!app.UpdateAndRender()) {}
    return 0;
}