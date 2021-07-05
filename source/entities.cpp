#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "entities.h"

/*
 * HoleEntity
 */
HoleEntity::HoleEntity(SDL_Texture** texture, int x, int y) {

    // Set texture and rect
    mTexture = texture;
    mRect.x = x;
    mRect.y = y;
    mRect.w = 64;
    mRect.h = 64;

    // Initialize state to resting
    mState = ST_RESTING;

    // Initialize resting and whacked timers
    srand(time(0));
    mRestingDuration = rand() % 3000 + 1000;
    mRestingTimer = SDL_GetTicks();
    mWhackedDuration = 1500;
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

    /* Algorithm
     * 1. Count down until next active state
     * 2. Set state to ST_ACTIVE
     * 3. Wait to get whacked
     * 4. Set state to ST_WHACKED
     * 5. Set state to ST_RESTING after some time
     * 6. Set random timer until next active state
     * 7. Go to step 1.
     */
}

void HoleEntity::draw(SDL_Renderer*& renderer) {
    SDL_RenderCopy(renderer, *mTexture, NULL, &mRect);
}

const SDL_Rect* HoleEntity::getRect() {
    return &mRect;
}

void HoleEntity::whack() {
    if(mState == ST_ACTIVE) {
        // Reset whacked timer
        mWhackedTimer = SDL_GetTicks();

        // Change state to whacked
        mState = ST_WHACKED;
    }
}

/*
 * State methods
 */
void HoleEntity::doResting() {
    printf("Resting.\n");

    // Count down until 'resting state' duration
    int now = SDL_GetTicks();
    if(now - mRestingTimer > mRestingDuration) {

        // Reset resting and active timers
        mRestingTimer = now;
        srand(time(0));
        mActiveDuration = rand() % 5000 + 1000;
        mActiveTimer = SDL_GetTicks();

        // Change state to active
        mState = ST_ACTIVE;
    }
}

void HoleEntity::doActive() {
    printf("Active.\n");

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
    printf("Whacked.%d | %d\n", SDL_GetTicks(), mWhackedTimer);

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
