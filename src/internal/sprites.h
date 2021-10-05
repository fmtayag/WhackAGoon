#ifndef SPRITES_H
#define SPRITES_H
#pragma once

#include <map>
#include <vector>
#include <iterator>

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

class ISprite {
    virtual void update() = 0;
    virtual void draw(SDL_Renderer* renderer) = 0;
};

class HoleSprite: public ISprite {
public:
    HoleSprite(SDL_Texture* spritesheet, int x, int y);
    ~HoleSprite(){};
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

    SDL_Texture* mSpritesheet;
    SDL_Rect m_Rect;

    int anim_timer;
    const int WHACKED_DUR = 1300;
    int whacked_timer;
    int awake_timer;
    int awake_dur;
	int m_CurFrame;
};

#endif // SPRITES_H
