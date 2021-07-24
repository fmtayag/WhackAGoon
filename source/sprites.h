#ifndef SPRITES_H
#define SPRITES_H
#pragma once

///////////////////////
class AbstractSprite {
    virtual void update() = 0;
    virtual void draw(SDL_Renderer*& renderer) = 0;
};

///////////////////////
class HoleSprite {
public:
    HoleSprite(SDL_Texture** textures[], int x, int y);
    ~HoleSprite();
    void update();
    void draw(SDL_Renderer*& renderer);

private:
    int mState;
    int mType;

    SDL_Texture* mTexture;
    SDL_Rect mRect;
    SDL_Rect mClip;
};

///////////////////////
class HammerSprite {
    HammerSprite();
    ~HammerSprite();
    void update();
    void draw(SDL_Renderer*& renderer);

private:
    int mState;

    SDL_Texture* mTexture;
    SDL_Rect mRect;
    SDL_Rect mClip;
};

///////////////////////
class SpawnManager {
public:
    SpawnManager();
    ~SpawnManager();

    void update();
};

#endif // SPRITES_H
