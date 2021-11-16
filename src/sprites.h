#ifndef SPRITES_H
#define SPRITES_H
#pragma once

#include <functional>
#include <list>
#include <map>
#include <SDL.h>
#include "g_data.h"
#include "widgets.h"

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

#endif // SPRITES_H