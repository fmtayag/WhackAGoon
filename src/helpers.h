#ifndef HELPERS_H
#define HELPERS_H
#pragma once

#include <iostream>
#include <SDL.h>

SDL_Texture *loadTextureFromFile(SDL_Renderer *renderer, std::string path);
const bool isPointCollide(SDL_Point point, SDL_Rect box);

#endif // HELPERS_H