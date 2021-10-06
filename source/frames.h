#ifndef FRAMES_H
#define FRAMES_H
#pragma once

#include <map>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

extern std::map<std::string, std::vector<SDL_Rect>> frm_ClipDat;
extern std::map<std::string, int> frm_AnimDelayDat;

void loadFrameData();

#endif // FRAMES_H
