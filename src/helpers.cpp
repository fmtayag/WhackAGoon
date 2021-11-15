#include <iostream>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "helpers.h"

SDL_Texture *loadTextureFromFile(SDL_Renderer *renderer, std::string path)
{
    // SDL_Surface *tempSurface = NULL;
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());

    // tempSurface = IMG_Load(path.c_str());

    // if (tempSurface == NULL)
    // {
    //     printf("SDL_image could not load image. Error: %s.\n", IMG_GetError());
    // }
    // else
    // {
    //     texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    //     if (texture == NULL)
    //     {
    //         printf("SDL could not create texture from surface. Error: %s.\n", SDL_GetError());
    //     }

    //     SDL_FreeSurface(tempSurface);
    //     tempSurface = NULL;
    // }

    return texture;
}
const bool isPointCollide(SDL_Point point, SDL_Rect box)
{
    int px = point.x;
    int py = point.y;
    int bx = box.x;
    int by = box.y;
    int bw = box.w;
    int bh = box.h;

    if (px <= bx || px >= bx + bw)
        return false;
    else if (py <= by || py >= by + bh)
        return false;

    return true;
}