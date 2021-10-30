#ifndef SPRITES_H
#define SPRITES_H
#pragma once

#include <map>
#include <vector>
#include <iterator>

enum AnimState
{
    AS_Resting,
    AS_ToAwake,
    AS_Awake,
    AS_Whacked,
    AS_ToResting
};

enum HoleType
{
    HT_None,
    HT_Goon,
    HT_Townie,
    HT_Mayor,
    HT_MaxNum
};

class ISprite
{
    virtual void update() = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;
};

class HoleSprite : public ISprite
{
public:
    HoleSprite(SDL_Texture *spritesheet, int x, int y);
    ~HoleSprite(){};
    void update();
    void draw(SDL_Renderer *renderer);

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

    SDL_Texture *mSpritesheet;
    SDL_Rect m_Rect;

    int anim_timer;
    static const int WHACKED_DUR = 1300;
    int whacked_timer;
    int awake_timer;
    int awake_dur;
    int m_CurFrame;
};

class Particle : public ISprite
{
public:
    Particle(SDL_Rect rect, SDL_Color color, SDL_Point velocity);
    ~Particle();

    void update();
    void draw(SDL_Renderer *renderer);

    bool isVisible();
    void bindParentTargTexture(SDL_Texture *parent) { m_parentTargTexture = parent; };

private:
    SDL_Rect m_rect;
    SDL_Color m_color;
    SDL_Point m_velocity;
    SDL_Texture *m_parentTargTexture;
    bool m_fdead = false;
};

class UI_Icon : public ISprite
{
public:
    UI_Icon(SDL_Texture *texture, SDL_Rect rect, int bob_delay);
    ~UI_Icon();

    void update();
    void draw(SDL_Renderer *renderer);

    void set_clip(SDL_Rect clip);
    const SDL_Rect get_rect() { return m_rect; };

private:
    SDL_Texture *m_texture;
    SDL_Rect m_rect;
    SDL_Rect m_clip;
    int m_origy; // remove the i for a good time...i need to grow up.
    int m_offsety;
    int delay_bobbing;
    int tmr_bobbing;
};

#endif // SPRITES_H
