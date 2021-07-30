#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "sprites.h"
#include "utils.h"

HoleSprite::HoleSprite(SDL_Texture* spritesheet, int x, int y) {
    mSpritesheet = spritesheet;
    mRect.x = x;
    mRect.y = y;
    mRect.w = 128;
    mRect.h = 128;

    m_CurFrame = 0;
    m_State = 1;
}

HoleSprite::~HoleSprite() {

}

// Update and draw
void HoleSprite::update() {

}

void HoleSprite::draw(SDL_Renderer* renderer) {

    SDL_RenderCopy(renderer, mSpritesheet, NULL, &mRect);
}

// Debug
void HoleSprite::nextFrame() {
    if(m_CurFrame < 7) {
        m_CurFrame++;
    }
    else {
        m_CurFrame = 0;
    }
}

void HoleSprite::prevFrame() {
    if(m_CurFrame > 0) {
        m_CurFrame--;
    }
    else {
        m_CurFrame = 0;
    }
}

int HoleSprite::getCurrentFrame() {
    return m_CurFrame;
}
