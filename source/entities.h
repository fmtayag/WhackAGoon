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

private:
    SDL_Texture** mTexture;
    SDL_Rect mRect;
    int mState;
    enum ENTITY_STATES {
        ST_RESTING,
        ST_ACTIVE,
        ST_WHACKED
    };
};

#endif // ENTITIES_H
