#include <iostream>
#include <SDL2/SDL.h>
#include "entities.h"

/*
 * GoonEntity
 */
GoonEntity::GoonEntity(SDL_Texture* texture, int x, int y) {
    mTexture = texture;
    mRect.x = x;
    mRect.y = y;
    mRect.w = 128;
    mRect.h = 128;
}

void GoonEntity::update() {
    // Update
}

void GoonEntity::draw(SDL_Renderer*& renderer) {
    SDL_RenderCopy(renderer, mTexture, NULL, &mRect);
}
