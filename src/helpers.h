#ifndef HELPERS_H
#define HELPERS_H
#pragma once

#include <iostream>
#include <SDL.h>
#include "g_data.h"

const bool isPointCollide(SDL_Point point, SDL_Rect box);
void centerPos(SDL_Point &pos, SDL_Rect &size, PosCentering poscenter);
void dbgPrint(DPL dblevel, std::string msg);

#endif // HELPERS_H