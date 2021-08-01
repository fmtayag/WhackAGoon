#include <iostream>
#include <map>
#include <tuple>
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
    m_State = AS_Awake;
    m_Type = HT_Goon;

    // set spritesheet clips

    // Resting animation
    mpClips[Z_ClipID(AS_Resting, HT_None)] = {
        {0, 16, 16, 16}
    };


    // For Goon
    mpClips[Z_ClipID(AS_ToAwake, HT_Goon)] = {
        {0, 32, 16, 16},
        {16, 32, 16, 16},
        {32, 32, 16, 16},
        {48, 32, 16, 16},
        {64, 32, 16, 16},
        {80, 32, 16, 16},
        {96, 32, 16, 16},
        {112, 32, 16, 16}
    };
    mpClips[Z_ClipID(AS_Awake, HT_Goon)] = {
        {0, 48, 16, 16},
        {16, 48, 16, 16},
        {32, 48, 16, 16},
        {48, 48, 16, 16}
    };
    mpClips[Z_ClipID(AS_Whacked, HT_Goon)] = {
        {0, 64, 16, 16},
        {16, 64, 16, 16},
        {32, 64, 16, 16},
        {48, 64, 16, 16}
    };
    mpClips[Z_ClipID(AS_ToResting, HT_Goon)] = {
        {112, 32, 16, 16},
        {96, 32, 16, 16},
        {80, 32, 16, 16},
        {64, 32, 16, 16},
        {48, 32, 16, 16},
        {32, 32, 16, 16},
        {16, 32, 16, 16},
        {0, 32, 16, 16}
    };


    // For Townie
    /** TODO **/


    // For Mayor
    /** TODO **/


}

HoleSprite::~HoleSprite() {

}

void HoleSprite::update() {

}




void HoleSprite::draw(SDL_Renderer* renderer) {

    SDL_RenderCopy(renderer,
                   mSpritesheet,
                   &mpClips[ Z_ClipID(m_State, m_Type) ][m_CurFrame],
                   &mRect);
}





// Debug
void HoleSprite::nextFrame() {
    /** TODO: Rework this with a vector iterator **/

}

void HoleSprite::prevFrame() {
    /** TODO: Rework this with a vector iterator **/

}
