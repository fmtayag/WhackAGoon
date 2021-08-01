#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "utils.h"
#include "sprites.h"

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






void drawText(SDL_Renderer* renderer, std::string message, TTF_Font* font, int x, int y, SDL_Color color) {
    // Create temp surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message.c_str(), color);

    // Create rect
    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};

    // Create texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Free temp surface
    SDL_FreeSurface(textSurface);

    // Draw texture
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    // Destroy texture
    SDL_DestroyTexture(texture);
}





void cleanUpTexture(SDL_Texture*& texture) {
    SDL_DestroyTexture(texture);
    texture = NULL;
}





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





std::string Z_ClipID(AnimState state, HoleType type) {
    // turn state, and type into a key string
    return std::to_string(state) + std::to_string(type);
}
