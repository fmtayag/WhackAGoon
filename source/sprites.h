#ifndef SPRITES_H
#define SPRITES_H
#pragma once

#include <vector>

/////////////////////// Abstract Sprite
class AbstractSprite {
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
};











/////////////////////// Hole Sprite
enum H_STATE {
    HST_RESTING,
    HST_TOACTIVE,
    HST_ACTIVE,
    HST_WHACKED,
    HST_TORESTING
};

enum H_TYPE {
    HTY_GOON,
    HTY_TOWNIE,
    HTY_MAYOR
};

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
    int m_State;
    int m_Type;
    int m_CurFrame;

    SDL_Texture* mSpritesheet;
    SDL_Rect mRect;
    SDL_Rect* m_CurClip;
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
