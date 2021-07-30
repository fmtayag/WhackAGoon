#include <iostream>
#include <SDL2/SDL.h>
#include "sprites.h"

HoleSprite::HoleSprite(SDL_Texture* spritesheet, int x, int y) {
    mSpritesheet = spritesheet;
    mRect.x = x;
    mRect.y = y;
    mRect.w = 128;
    mRect.h = 128;

    m_CurrentFrame = 0;
}

HoleSprite::~HoleSprite() {

}

// Update and draw
void HoleSprite::update() {

}

void HoleSprite::draw(SDL_Renderer* renderer) {
    SDL_Rect R_Clips[8] = {
        {0, 32, 16, 16},
        {16, 32, 16, 16},
        {32, 32, 16, 16},
        {48, 32, 16, 16},
        {64, 32, 16, 16},
        {80, 32, 16, 16},
        {96, 32, 16, 16},
        {112, 32, 16, 16},
    };

    SDL_RenderCopy(renderer, mSpritesheet, &R_Clips[m_CurrentFrame], &mRect);
}

// Debug
void HoleSprite::nextFrame() {
    if(m_CurrentFrame < 7) {
        m_CurrentFrame++;
    }
    else {
        m_CurrentFrame = 0;
    }
}

void HoleSprite::prevFrame() {
    if(m_CurrentFrame > 0) {
        m_CurrentFrame--;
    }
    else {
        m_CurrentFrame = 0;
    }
}

int HoleSprite::getCurrentFrame() {
    return m_CurrentFrame;
}
