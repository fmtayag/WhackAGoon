#ifndef ENTITIES_H
#define ENTITIES_H
#pragma once

#include <SDL2/SDL.h>

class AbstractEntity {
public:
    virtual void update()=0;
    virtual void draw(SDL_Renderer*& renderer) = 0;
    virtual const SDL_Rect* getRect() = 0;
};

class HoleEntity: public AbstractEntity {
public:
    HoleEntity(SDL_Texture** texture, int x, int y);
    void update();
    void draw(SDL_Renderer*& renderer);
    const SDL_Rect* getRect();
    void whack();

    // State methods
    void doResting();
    void doActive();
    void doWhacked();

private:
    // Texture and rect
    SDL_Texture** mTexture;
    SDL_Rect mRect;

    // States
    int mState;
    enum ENTITY_STATES {
        ST_RESTING,
        ST_ACTIVE,
        ST_WHACKED
    };

    // Timers
    int mRestingTimer;
    int mRestingDuration;
    int mActiveTimer;
    int mActiveDuration;
    int mWhackedTimer;
    int mWhackedDuration;
};

#endif // ENTITIES_H
