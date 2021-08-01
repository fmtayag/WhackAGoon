#ifndef SPRITES_H
#define SPRITES_H
#pragma once

#include <map>
#include <vector>
#include <iterator>

/////////////////////// Abstract Sprite
class AbstractSprite {
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
};











/////////////////////// Hole Sprite
enum AnimState {
    AS_Resting,
    AS_ToAwake,
    AS_Awake,
    AS_Whacked,
    AS_ToResting
};

enum HoleType {
    HT_None,
    HT_Goon,
    HT_Townie,
    HT_Mayor
};

class HoleSprite {
public:
    HoleSprite(SDL_Texture* spritesheet, int x, int y);
    ~HoleSprite();
    void update();
    void draw(SDL_Renderer* renderer);

    void animate();
    void whack();
    void awake(HoleType hType = HT_Goon);

    // Debug
    void nextFrame();
    void prevFrame();
    int getCurFrame() { return m_CurFrame; }
    int getType() { return m_Type; }
    int getAnimState() { return m_AnimState; }

private:
    AnimState m_AnimState;
    HoleType m_Type;
    int m_CurFrame;

    SDL_Texture* mSpritesheet;
    SDL_Rect mRect;
    SDL_Rect* m_CurClip;
    std::map<std::string, std::vector<SDL_Rect>> mpClips;

    int anim_timer;
    std::map<std::string, int> mpAnimDelays;
};














/////////////////////// Hammer Sprite
enum HAM_STATE {
    HAM_ST_UNSMASHED,
    HAM_ST_SMASHED
};

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
