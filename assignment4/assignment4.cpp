//
//  assignment4.cpp
//  Assignment4
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "PlatformerApp.h"

int main(int argc, char *argv[])
{
    PlatformerApp app;
    while(!app.UpdateAndRender()) {}
    return 0;
}
