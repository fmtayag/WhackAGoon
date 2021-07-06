#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"

/* -------------------------------------------------
 * loadTextureFromFile
 * -------------------------------------------------
 */
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

/* -------------------------------------------------
 * cleanUpTexture
 * -------------------------------------------------
 */
void cleanUpTexture(SDL_Texture*& texture) {
    SDL_DestroyTexture(texture);
    texture = NULL;
}

/* -------------------------------------------------
 * isPointCollide
 * -------------------------------------------------
 */
bool isPointCollide(int mpos[2], const SDL_Rect* rect) {
    int mx = mpos[0];
    int my = mpos[1];
    int rx = rect->x;
    int ry = rect->y;
    int rw = rect->w;
    int rh = rect->h;

    if(mx <= rx || mx >= rx + rw)
        return false;
    else if(my <= ry || my >= ry + rh)
        return false;

    return true;
}
