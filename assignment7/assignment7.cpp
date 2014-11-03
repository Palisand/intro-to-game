//
//  assignment7.cpp
//  Assignment7
//

//Change to Asteroids app

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "AsteroidsApp.h"

int main(int argc, char *argv[])
{
    AsteroidsApp app;
    while(!app.UpdateAndRender()) {}
    return 0;
}