#include <iostream>
#include <functional>
#include <map>
#include <cassert>
#include <SDL2/SDL.h>
#include "sprites.h"
#include "g_data.h"
#include "helpers.h"
#include "g_texture.h"

#pragma region GButton
//{ GButton
GButton::GButton(GTexture *texture, SDL_Rect rect, std::map<BtnState, SDL_Rect> clips)
{
    // m_texture = new GTexture();
    // m_texture->loadFromFile(imgpath);
    m_texture = texture;
    m_rect.x = rect.x;
    m_rect.y = rect.y;
    m_rect.w = rect.w;
    m_rect.h = rect.h;
    //m_state = BST_NORMAL; // set at header instead
    m_clips = clips;

    WindowMetadata winDat;

    if (m_rect.w <= 0)
        m_rect.w = m_clips[m_state].w * winDat.PXSCALE;
    if (m_rect.h <= 0)
        m_rect.h = m_clips[m_state].h * winDat.PXSCALE;
}
GButton::~GButton()
{
    delete m_texture;
    printf("GButton::~GButton() | GButton deleted.\n");
}
void GButton::update(GameMouse *mouse_s)
{
    u_state(mouse_s);

    if (m_state == BST_CLICKED)
    {
        makeCallback();
        m_state = BST_NORMAL;
        mouse_s->isClicked = false;
    }
}
void GButton::u_state(GameMouse *mouse_s)
{
    bool collided = isPointCollide(mouse_s->position, this->getRect());
    if (collided)
    {
        this->setState(BST_HOVERED);

        if (mouse_s->isClicked)
        {
            this->setState(BST_CLICKED);
        }
    }
    else
    {
        this->setState(BST_NORMAL);
    }
}
void GButton::draw()
{
    //printf("m_clips rect: (%d, %d, %d, %d) (xywh).\n", m_clips[m_state].x, m_clips[m_state].y, m_clips[m_state].w, m_clips[m_state].h);
    m_texture->draw(&m_clips[m_state], &m_rect);
}
void GButton::setState(BtnState state)
{
    if (m_state != BST_DISABLED)
    {
        m_state = state;
    }
}
void GButton::bindCallback(std::function<void()> cback)
{
    m_callback = cback;
}
void GButton::makeCallback()
{
    if (m_callback != NULL)
    {
        m_callback();
    }
}
//}
#pragma endregion GButton