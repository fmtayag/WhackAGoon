#ifndef SPRITES_H
#define SPRITES_H
#pragma once

/////////////////////// Abstract Sprite
class AbstractSprite {
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
};







/////////////////////// Hole Sprite
class HoleSprite {
public:
    HoleSprite(SDL_Texture* spritesheet, int x, int y);
    ~HoleSprite();
    void update();
    void draw(SDL_Renderer* renderer);

    // Debug
    void nextFrame();
    void prevFrame();
    int getCurrentFrame();

private:
    int mState;
    int mType;
    int m_CurrentFrame;

    SDL_Texture* mSpritesheet;
    SDL_Rect mRect;
    SDL_Rect mClip;
};









/////////////////////// Hammer Sprite
class HammerSprite {
    HammerSprite();
    ~HammerSprite();
    void update();
    void draw(SDL_Renderer* renderer);

private:
    int mState;

    SDL_Texture* mTexture;
    SDL_Rect mRect;
    SDL_Rect mClip;
};










/////////////////////// Hole Manager
class HoleManager {
public:
    HoleManager();
    ~HoleManager();

    void update();
};

#endif // SPRITES_H
