//
//  Sound.cpp
//  FinalAssignment
//

#include "Sound.h"
#include <iostream>

Sound::Sound(std::string filePath) {
    chunk = nullptr;
    chunk = Mix_LoadWAV(filePath.c_str());
    this->filePath = filePath;
}

//Sound::~Sound() {
//    std::cout << filePath << " DESTROYED" << std::endl;
//}

void Sound::Quit() {
    Mix_FreeChunk(chunk);
}

void Sound::Play(int channel, int loop) {
    Mix_PlayChannel(channel, chunk, loop);
}

void Sound::SetVolume(int volume) {
    Mix_VolumeChunk(chunk, volume);
}