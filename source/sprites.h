#ifndef SPRITES_H
#define SPRITES_H
#pragma once

#include <map>
#include <vector>
#include <iterator>

/////////////////////// sprite interf
class ISprite {
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
    HT_Mayor,
    HT_MaxNum
};

class HoleSprite: public ISprite {
public:
    HoleSprite(SDL_Texture* spritesheet, int x, int y);
    ~HoleSprite();
    void update();
    void draw(SDL_Renderer* renderer);

    void animate();
    bool whack();
    void awake(HoleType hType = HT_Goon);

    int getCurFrame() { return m_CurFrame; }
    int getType() { return m_Type; }
    int getAnimState() { return m_AnimState; }
    SDL_Rect getRect() { return m_Rect; }

private:
    AnimState m_AnimState;
    HoleType m_Type;
    int m_CurFrame;

    SDL_Texture* mSpritesheet;
    SDL_Rect m_Rect;
    SDL_Rect* m_CurClip;

    int anim_timer;
    const int WHACKED_DUR = 1300;
    int whacked_timer;
    int awake_timer;
    int awake_dur;

    std::map<std::string, std::vector<SDL_Rect>> frames_Clips;
    std::map<std::string, int> frames_AnimDelays;
};














/////////////////////// Hammer Sprite
enum HAM_STATE {
    HAM_ST_UNSMASHED,
    HAM_ST_SMASHED
};

class HammerSprite: public ISprite {
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
    HoleManager(std::vector<HoleSprite*>& holes);
    ~HoleManager();

    void update();

private:
    std::vector<HoleSprite*>* m_Holes;
    int wakeUpDelay;
    int wakeUpTimer;
};

#endif // SPRITES_H
