#include <iostream>
#include <SDL2/SDL.h>
#include "entities.h"

/*
 * HoleEntity
 */
HoleEntity::HoleEntity(SDL_Texture** texture, int x, int y) {
    mTexture = texture;
    mRect.x = x;
    mRect.y = y;
    mRect.w = 64;
    mRect.h = 64;
    mState = ST_ACTIVE;
}

void HoleEntity::update() {
    if(mState == ST_WHACKED)
        printf("I'm whacked.\n");
}

void HoleEntity::draw(SDL_Renderer*& renderer) {
    SDL_RenderCopy(renderer, *mTexture, NULL, &mRect);
}

const SDL_Rect* HoleEntity::getRect() {
    return &mRect;
}

void HoleEntity::whack() {
    if(mState == ST_ACTIVE) {
        mState = ST_WHACKED;
    }
}
