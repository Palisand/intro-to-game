//
//  Sound.h
//  FinalAssignment
//

#ifndef __FinalAssignment__Sound__
#define __FinalAssignment__Sound__

#include "SDL_mixer.h"
#include <string>

class Sound {
public:
    Sound(std::string);
//    ~Sound();
    void Quit();
    void Play(int channel = -1, int loop = 0);
    void SetVolume(int volume);
    
private:
    Mix_Chunk* chunk;
    std::string filePath;
};

#endif /* defined(__FinalAssignment__Sound__) */
