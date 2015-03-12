//
//  main.cpp
//  FinalAssignment
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Application.h"

int main(int argc, char *argv[])
{
    Application app;
    while(!app.UpdateAndRender()) {}
    return 0;
}
