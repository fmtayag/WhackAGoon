#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <random>
#include <ctime>
#include <cstddef>
#include <string>
#include <SDL2/SDL_ttf.h>
#include "sprites.h"

// Loading assets
SDL_Texture* loadTextureFromFile(SDL_Renderer* renderer, std::string path);

// Rendering texts
void drawText(SDL_Renderer* renderer, std::string message, TTF_Font* font, int x, int y, SDL_Color color, bool cenX=false);

// Destroying assets utils
void cleanUpTexture(SDL_Texture*& texture);

// Collision stuff
bool isPointCollide(int point[2], SDL_Rect rect);

// RNG utils
template <std::size_t N, std::size_t M>
int pickWeighted(const int (&numbers)[N], const int (&weights)[M]) {
    std::piecewise_constant_distribution<> dist(
        std::begin(numbers),
        std::end(numbers),
        std::begin(weights)
    );

    std::mt19937 generator(time(0));
    int randNum = static_cast<unsigned>(dist(generator));
    return randNum;
}

// For getting size of array pointer
template<typename T, size_t SIZE>
size_t getSize(T (&)[SIZE]) {
    return SIZE;
}

// turn state, and type into a key string
std::string Z_ClipID(AnimState state, HoleType type);

#endif // UTILS_H
