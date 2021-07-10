#ifndef ENTITIES_H
#define ENTITIES_H
#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

/* -------------------------------------------------
 * Abstract Entity
 * -------------------------------------------------
 */
class AbstractEntity {
public:
    virtual void update()=0;
    virtual void draw(SDL_Renderer*& renderer) = 0;
    virtual const SDL_Rect* getRect() = 0;
};

/* -------------------------------------------------
 * Hole Entity
 * -------------------------------------------------
 */

enum ENTITY_STATES {
    ST_RESTING,
    ST_ACTIVE,
    ST_WHACKED
};

enum HOLE_TYPES {
    TYPE_GOON,
    TYPE_TOWNIE,
    TYPE_MAYOR,
    TYPE_MAXNO
};

class HoleEntity: public AbstractEntity {
public:
    HoleEntity(SDL_Texture** textures[], int x, int y);
    ~HoleEntity();
    void update();
    void draw(SDL_Renderer*& renderer);
    bool whack();

    // State methods
    void doResting();
    void doActive();
    void doWhacked();

    // Getters
    int getType() {return mType;}
    SDL_Rect* getRect() {return &mRect;}

private:
    // Texture and rect
    SDL_Texture** mTextures;
    SDL_Texture* mTexture;
    SDL_Rect mRect;
    SDL_Rect mClips[3];

    // State
    int mState;

    // Type
    int mType;

    // Timers
    int mRestingTimer;
    int mRestingDuration;
    int mActiveTimer;
    int mActiveDuration;
    int mWhackedTimer;
    int mWhackedDuration;

    void chooseType();
};

/* -------------------------------------------------
 * Hammer entity
 * -------------------------------------------------
 */
class HammerEntity: public AbstractEntity {
public:
    HammerEntity(SDL_Texture** texture);
    void update();
    void draw(SDL_Renderer*& renderer);
    const SDL_Rect* getRect();
    void smash();
    void unsmash();

private:
    // Texture and rects
    SDL_Texture** mTexture;
    SDL_Rect mRect;
    SDL_Rect mClips[2];

    // States
    int mState;
    enum ENTITY_STATES {
        ST_UNSMASHED,
        ST_SMASHED
    };

    // Animation timer
    int animationTimer;
    int animationDelay;
};

#endif // ENTITIES_H
