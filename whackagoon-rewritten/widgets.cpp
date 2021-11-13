#include <iostream>
#include <SDL2/SDL.h>
#include "widgets.h"

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