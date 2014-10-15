//
//  assignment5.cpp
//  Assignment5
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "SidescrollerApp.h"

int main(int argc, char *argv[])
{
    SidescrollerApp app;
    while(!app.UpdateAndRender()) {}
    return 0;
}