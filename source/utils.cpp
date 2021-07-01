#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"

SDL_Texture* loadTextureFromFile(SDL_Renderer* renderer, std::string path) {
    SDL_Surface* tempSurface = NULL;
    SDL_Texture* texture = NULL;

    tempSurface = IMG_Load(path.c_str());

    if(tempSurface == NULL) {
        printf("SDL_image could not load image. Error: %s.\n", IMG_GetError());

    }
    else {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

        if(texture == NULL) {
            printf("SDL could not create texture from surface. Error: %s.\n", SDL_GetError());
        }

        SDL_FreeSurface(tempSurface);
        tempSurface = NULL;

    }

    return texture;
}
