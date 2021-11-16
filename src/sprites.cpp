#include <iostream>
#include <functional>
#include <map>
#include <cassert>
#include <algorithm>
#include <vector>
#include <SDL.h>
#include "sprites.h"
#include "g_data.h"
#include "helpers.h"

#pragma region GButton
//{ GButton
GButton::GButton(GTexture *texture, SDL_Rect rect, std::map<BtnState, SDL_Rect> clips)
{
    // m_texture = new GTexture();
    // m_texture->loadFromFile(imgpath);

    // Reminder: don't bother deep copying this...
    // You can deep copy the GTexture object but not the SDL_Texture inside it
    // Incomplete types like SDL_Texture can't be deep copied, it seems.
    m_texture = texture;

    m_rect.x = rect.x;
    m_rect.y = rect.y;
    m_rect.w = rect.w;
    m_rect.h = rect.h;
    m_state = BST_NORMAL;
    m_clips = clips;

    WindowMetadata winDat;

    if (m_rect.w <= 0)
        m_rect.w = m_clips[m_state].w * winDat.PXSCALE;
    if (m_rect.h <= 0)
        m_rect.h = m_clips[m_state].h * winDat.PXSCALE;
}
GButton::~GButton()
{
    m_texture = NULL;
    printf("GButton::~GButton() | GButton deleted.\n");
}
void GButton::update(GameMouse *mouse_s)
{
    if (this != nullptr)
    {
        u_state(mouse_s);

        if (m_state == BST_CLICKED)
        {
            makeCallback();
            m_state = BST_NORMAL;
            mouse_s->isClicked = false;
        }
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
    if (this != nullptr)
    {
        //printf("m_clips rect: (%d, %d, %d, %d) (xywh).\n", m_clips[m_state].x, m_clips[m_state].y, m_clips[m_state].w, m_clips[m_state].h);
        m_texture->draw(&m_clips[m_state], &m_rect);
    }
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

#pragma region Particle
Particle::Particle(SDL_Rect rect, Vector2 velocity, SDL_Color color)
{
    m_rect = rect;
    m_velocity = velocity;
    m_color = color;
}
Particle::~Particle()
{
    //printf("Particle deleted.\n");
}
void Particle::update()
{
    m_rect.x += m_velocity.x;
    m_rect.y += m_velocity.y;
}
void Particle::draw()
{
    SDL_SetRenderDrawColor(gameRenderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(gameRenderer, &m_rect);
}
SDL_Point Particle::getPos()
{
    SDL_Point pos;
    pos.x = m_rect.x;
    pos.y = m_rect.y;

    return pos;
}
#pragma endregion Particle

#pragma region Hole
Hole::Hole(std::shared_ptr<GTexture> texture, SDL_Point pos, PosCentering poscenter)
{
    // Initialize m_sheetClips (static)

    WindowMetadata winData;
    m_texture = texture;

    m_rect.x = pos.x;
    m_rect.y = pos.y;
    m_rect.w = 16 * winData.PXSCALE;
    m_rect.h = 16 * winData.PXSCALE;

    // Check for position centers
    switch (poscenter)
    {
    case PosCentering::POSCEN_NONE:
        // Do nothing
        break;
    case PosCentering::POSCEN_X:
        m_rect.x = (pos.x / 2) - (m_rect.w / 2);
        break;
    case PosCentering::POSCEN_Y:
        m_rect.y = pos.y - (m_rect.h / 2);
        break;
    case PosCentering::POSCEN_BOTH:
        m_rect.x = pos.x - (m_rect.w / 2);
        m_rect.y = pos.y - (m_rect.h / 2);
        break;
    default:
        printf("WARNING: Hole::Hole() -> default case reached.\n");
        break;
    }
}
Hole::~Hole()
{
    m_texture = NULL;
}
void Hole::update()
{
}
void Hole::draw()
{
    m_state = HoleState(HT_MAYOR, HAS_TOREST);
    m_texture->draw(&m_sheetMap[m_state][1], &m_rect);
}
Hole::SheetMap Hole::createSheetMap()
{
    SheetMap smap;

    // Empty hole sprite
    smap[HoleState(HT_NONE, HAS_REST)] = {
        {0, 16, 16, 16}};

    // --- Goon sprites ---
    smap[HoleState(HT_GOON, HAS_TOAWAKE)] = {
        {0, 32, 16, 16},
        {16, 32, 16, 16},
        {32, 32, 16, 16},
        {48, 32, 16, 16},
        {64, 32, 16, 16},
        {80, 32, 16, 16},
        {96, 32, 16, 16},
        {112, 32, 16, 16}};

    smap[HoleState(HT_GOON, HAS_AWAKE)] = {
        {0, 48, 16, 16},
        {16, 48, 16, 16},
        {32, 48, 16, 16},
        {48, 48, 16, 16}};

    smap[HoleState(HT_GOON, HAS_WHACKED)] = {
        {0, 64, 16, 16},
        {16, 64, 16, 16},
        {32, 64, 16, 16},
        {48, 64, 16, 16}};

    // ... just reverse the "to awake" frames for the "to rest" frames.
    smap[HoleState(HT_GOON, HAS_TOREST)] = smap[HoleState(HT_GOON, HAS_TOAWAKE)];
    std::reverse(smap[HoleState(HT_GOON, HAS_TOREST)].begin(), smap[HoleState(HT_GOON, HAS_TOREST)].end());

    // --- Townie sprites ---
    smap[HoleState(HT_TOWNIE, HAS_TOAWAKE)] = {
        {0, 128, 16, 16},
        {16, 128, 16, 16},
        {32, 128, 16, 16},
        {48, 128, 16, 16},
        {64, 128, 16, 16},
        {80, 128, 16, 16},
        {96, 128, 16, 16},
        {112, 128, 16, 16}};

    smap[HoleState(HT_TOWNIE, HAS_AWAKE)] = {
        {0, 144, 16, 16},
        {16, 144, 16, 16},
        {32, 144, 16, 16},
        {48, 144, 16, 16},
        {64, 144, 16, 16},
        {80, 144, 16, 16}};

    smap[HoleState(HT_TOWNIE, HAS_WHACKED)] = {
        {0, 160, 16, 16},
        {16, 160, 16, 16},
        {32, 160, 16, 16},
        {48, 160, 16, 16}};

    // ... just reverse the "to awake" frames for the "to rest" frames.
    smap[HoleState(HT_TOWNIE, HAS_TOREST)] = smap[HoleState(HT_TOWNIE, HAS_TOAWAKE)];
    std::reverse(smap[HoleState(HT_TOWNIE, HAS_TOREST)].begin(), smap[HoleState(HT_TOWNIE, HAS_TOREST)].end());

    // --- Mayor sprites ---
    smap[HoleState(HT_MAYOR, HAS_TOAWAKE)] = {
        {0, 80, 16, 16},
        {16, 80, 16, 16},
        {32, 80, 16, 16},
        {48, 80, 16, 16},
        {64, 80, 16, 16},
        {80, 80, 16, 16},
        {96, 80, 16, 16},
        {112, 80, 16, 16}};

    smap[HoleState(HT_MAYOR, HAS_AWAKE)] = {
        {0, 96, 16, 16},
        {16, 96, 16, 16},
        {32, 96, 16, 16},
        {48, 96, 16, 16}};

    smap[HoleState(HT_MAYOR, HAS_WHACKED)] = {
        {0, 112, 16, 16},
        {16, 112, 16, 16},
        {32, 112, 16, 16},
        {48, 112, 16, 16}};

    // ... just reverse the "to awake" frames for the "to rest" frames.
    smap[HoleState(HT_MAYOR, HAS_TOREST)] = smap[HoleState(HT_MAYOR, HAS_TOAWAKE)];
    std::reverse(smap[HoleState(HT_MAYOR, HAS_TOREST)].begin(), smap[HoleState(HT_MAYOR, HAS_TOREST)].end());

    return smap;
}
Hole::SheetMap Hole::m_sheetMap = Hole::createSheetMap();
#pragma endregion Hole