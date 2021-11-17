#ifndef SPRITES_H
#define SPRITES_H
#pragma once

#include <functional>
#include <utility>
#include <list>
#include <map>
#include <vector>
#include <SDL.h>
#include "g_data.h"
#include "widgets.h"

enum PosCentering;

enum BtnState
{
    BST_DISABLED,
    BST_NORMAL,
    BST_CLICKED,
    BST_HOVERED
};

class GButton
{
private:
    SDL_Rect m_rect;
    GTexture *m_texture;
    BtnState m_state;
    std::map<BtnState, SDL_Rect> m_clips;
    std::function<void()> m_callback;

    void u_state(GameMouse *mouse_s);
    void setState(BtnState state);
    void makeCallback();

public:
    //GButton(std::string imgpath, SDL_Rect rect, std::map<BtnState, SDL_Rect> clips);
    GButton(GTexture *texture, SDL_Rect rect, std::map<BtnState, SDL_Rect> clips);
    ~GButton();
    void update(GameMouse *mouse_s);
    void draw();

    SDL_Rect getRect() { return m_rect; };
    void bindCallback(std::function<void()> cback);
};

class Particle
{
private:
    SDL_Rect m_rect;
    Vector2 m_velocity;
    SDL_Color m_color;

public:
    Particle(SDL_Rect rect, Vector2 velocity, SDL_Color color);
    ~Particle();
    void update();
    void draw();

    SDL_Point getPos();
};

class Hole
{
public:
    enum HoleType
    {
        HT_NONE,
        HT_GOON,
        HT_TOWNIE,
        HT_MAYOR
    };
    enum HoleAnimState
    {
        HAS_REST,
        HAS_TOAWAKE,
        HAS_AWAKE,
        HAS_WHACKED,
        HAS_TOREST,
    };
    enum HoleHitStatus
    {
        UNSET,
        MISS,
        HIT
    };

    Hole(std::shared_ptr<GTexture> texture, SDL_Point pos, PosCentering poscenter);
    ~Hole();
    void update(GameMouse &gMouse);
    void draw();

    void awaken(HoleType type);

    HoleHitStatus fetchHitStatus() { return m_hitStat; };
    HoleType fetchHoleType();

private:
    // Spritesheet map stuff
    typedef std::pair<HoleType, HoleAnimState> HoleState;
    typedef std::map<HoleState, std::vector<SDL_Rect>> SheetMap;
    static SheetMap m_sheetMap;
    static SheetMap createSheetMap();

    // Other crap
    std::shared_ptr<GTexture> m_texture;
    SDL_Rect m_rect;
    HoleState m_state;
    int m_curF;
    HoleHitStatus m_hitStat = UNSET;

    // AnimState stuff
    const int MAX_WHACKED_LOOP = 3;
    int whacked_loop = 0;

    // Timers
    GTimer m_tmrNxtF;
    Uint32 m_delayNxtF = 80;
    static const Uint32 m_SLOW_ANIM = 400;
    static const Uint32 m_NORM_ANIM = 60;
    GTimer m_tmrAwake;
    Uint32 m_delayAwake; // TODO: Randomize later.

    // Helper methods
    void whack();
    void nextAS();
    void setHitStatus(HoleHitStatus status) { m_hitStat = status; };
    void slowFrameDelay() { m_delayNxtF = m_SLOW_ANIM; };
    void normalFrameDelay() { m_delayNxtF = m_NORM_ANIM; };
};

#endif // SPRITES_H