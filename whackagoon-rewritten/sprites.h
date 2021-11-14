#ifndef SPRITES_H
#define SPRITES_H
#pragma once

#include <functional>
#include <list>
#include <map>
#include <SDL2/SDL.h>
#include "g_data.h"
#include "g_texture.h"

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
    BtnState m_state = BST_NORMAL;
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

#endif // SPRITES_H