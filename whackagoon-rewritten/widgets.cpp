#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "widgets.h"
#include "g_data.h"

#pragma region GTimer
GTimer::GTimer()
{
    m_startTicks = 0;
    m_pausedTicks = 0;
    m_state = GTS_STOPPED;
}
void GTimer::start()
{
    m_state = GTS_STARTED;
    m_startTicks = SDL_GetTicks();
    m_pausedTicks = 0;
}
void GTimer::stop()
{
    m_state = GTS_STOPPED;
    m_startTicks = 0;
    m_pausedTicks = 0;
}
void GTimer::pause()
{
    if (m_state == GTS_STARTED)
    {
        m_state = GTS_PAUSED;

        m_pausedTicks = SDL_GetTicks() - m_startTicks;
        m_startTicks = 0;
    }
}
void GTimer::unpause()
{
    if (m_state == GTS_PAUSED)
    {
        m_state = GTS_STARTED;
        m_startTicks = SDL_GetTicks() - m_pausedTicks;
        m_pausedTicks = 0;
    }
}
Uint32 GTimer::getTicks()
{
    Uint32 time = 0;
    if (m_state == GTS_STARTED)
        time = SDL_GetTicks() - m_startTicks;
    else if (m_state == GTS_PAUSED)
        time = m_pausedTicks;

    return time;
}
#pragma endregion GTimer

#pragma region GTexture
GTexture::GTexture()
{
    m_texture = NULL;
}
GTexture::~GTexture()
{
    free();
    printf("GTexture::~GTexture() | Deleted.\n");
}
void GTexture::loadFromFile(std::string path)
{
    m_texture = IMG_LoadTexture(gameRenderer, path.c_str());
    printf("GTexture::loadFromFile() | IMG_Error: %s\n", IMG_GetError());
}
void GTexture::draw(SDL_Rect *clip, SDL_Rect *dst)
{
    SDL_RenderCopy(gameRenderer, m_texture, clip, dst);
    // printf("GTexture::draw() | SDL_Error: %s\n", SDL_GetError());
    // printf("GTexture::draw() | IMG_Error: %s\n", IMG_GetError());
}
void GTexture::free()
{
    if (m_texture != NULL)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = NULL;
    }
}
#pragma endregion GTexture