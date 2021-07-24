#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "entities.h"
#include "utils.h"

/* -------------------------------------------------
 * Hole entity
 * -------------------------------------------------
 */
HoleEntity::HoleEntity(SDL_Texture** textures[], int x, int y) {
    // Point mTextures to array ref
    mTextures = *textures;

    // Initialize textures and rect
    mTexture = mTextures[TYPE_GOON];
    mRect.x = x;
    mRect.y = y;
    mRect.w = 128;
    mRect.h = 128;

    // Initialize image clips
    mClips[ST_RESTING].x = 0;
    mClips[ST_RESTING].y = 0;
    mClips[ST_RESTING].w = 16;
    mClips[ST_RESTING].h = 16;
    mClips[ST_ACTIVE].x = 16;
    mClips[ST_ACTIVE].y = 0;
    mClips[ST_ACTIVE].w = 16;
    mClips[ST_ACTIVE].h = 16;
    mClips[ST_WHACKED].x = 32;
    mClips[ST_WHACKED].y = 0;
    mClips[ST_WHACKED].w = 16;
    mClips[ST_WHACKED].h = 16;

    // Initialize state to resting
    mState = ST_RESTING;

    // Initialize resting and whacked timers
    mRestingDuration = rand() % 8000 + 1000;
    mRestingTimer = SDL_GetTicks();
    mWhackedDuration = 1000;

    // choose type
    chooseType();
}

HoleEntity::~HoleEntity() {
    mTextures = NULL;
    mTexture = NULL;
}

void HoleEntity::update() {
    switch(mState) {
    case ST_RESTING:
        doResting();
        break;
    case ST_ACTIVE:
        doActive();
        break;
    case ST_WHACKED:
        doWhacked();
        break;
    }
}

void HoleEntity::draw(SDL_Renderer*& renderer) {
    SDL_RenderCopy(renderer, mTextures[mType], &mClips[mState], &mRect);
}

bool HoleEntity::whack() {
    if(mState == ST_ACTIVE) {
        // Reset whacked timer
        mWhackedTimer = SDL_GetTicks();

        // Change state to whacked
        mState = ST_WHACKED;

        return true;
    }
    else {
        return false;
    }
}

void HoleEntity::chooseType() {
    // Choose a random number between 0 and TYPE_MAXNO (value 4)
    int interval[3] = {TYPE_GOON, TYPE_TOWNIE, TYPE_MAXNO};
    int weights[3] = {10, 3, 1};
    mType = pickWeighted(interval, weights);
}

void HoleEntity::doResting() {
    // Count down until 'resting state' duration
    int now = SDL_GetTicks();
    if(now - mRestingTimer > mRestingDuration) {

        // Reset resting and active timers
        mRestingTimer = now;
        srand(time(0));
        mActiveDuration = rand() % 2000 + 1000;
        mActiveTimer = SDL_GetTicks();

        // Choose next type
        chooseType();

        // Change state to active
        mState = ST_ACTIVE;
    }
}

void HoleEntity::doActive() {
    // Count down until 'active state' duration
    int now = SDL_GetTicks();
    if(now - mActiveTimer > mActiveDuration) {

        // Reset resting timer
        mRestingDuration = rand() % 3000 + 1000;
        mRestingTimer = now;

        // Change state to resting
        mState = ST_RESTING;
    }
}

void HoleEntity::doWhacked() {
    // Count down until 'whacked state' duration
    int now = SDL_GetTicks();
    if(now - mWhackedTimer > mWhackedDuration) {

        // Reset resting timer
        mRestingDuration = rand() % 3000 + 1000;
        mRestingTimer = now;

        // Change state to resting
        mState = ST_RESTING;
    }
}

/* -------------------------------------------------
 * Hammer entity
 * -------------------------------------------------
 */

HammerEntity::HammerEntity(SDL_Texture** texture) {
    // Initialize stuff
    mTexture = texture;
    mRect.x = 0;
    mRect.y = 0;
    mRect.w = 128;
    mRect.h = 128;

    // Initialize clips
    mClips[ST_UNSMASHED].x = 0;
    mClips[ST_UNSMASHED].y = 0;
    mClips[ST_UNSMASHED].w = 16;
    mClips[ST_UNSMASHED].h = 16;
    mClips[ST_SMASHED].x = 16;
    mClips[ST_SMASHED].y = 0;
    mClips[ST_SMASHED].w = 16;
    mClips[ST_SMASHED].h = 16;

    // Initialize animation timers
    animationTimer = SDL_GetTicks();
    animationDelay = 200;

    // initialize state
    mState = ST_UNSMASHED;
}

void HammerEntity::update() {
    if(mState == ST_SMASHED) {
        int now = SDL_GetTicks();
        if(now - animationTimer > animationDelay) {
            unsmash();
        }
    }
    else {
        animationTimer = SDL_GetTicks();
    }
}

void HammerEntity::draw(SDL_Renderer*& renderer) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    mRect.x = mx-32;
    mRect.y = my-32;

    SDL_RenderCopy(renderer, *mTexture, &mClips[mState], &mRect);
}

const SDL_Rect* HammerEntity::getRect() {
    return &mRect;
}

void HammerEntity::smash() {
    mState = ST_SMASHED;
}

void HammerEntity::unsmash() {
    mState = ST_UNSMASHED;
}
