#ifndef WIDGETS_H
#define WIDGETS_H
#pragma once

class GTimer
{
private:
    enum GTimer_State
    {
        GTS_STOPPED,
        GTS_PAUSED,
        GTS_STARTED
    } m_state;
    Uint32 m_startTicks;
    Uint32 m_pausedTicks;

public:
    GTimer();
    ~GTimer() = default;

    void start();
    void stop();
    void pause();
    void unpause();

    Uint32 getTicks();
};

#endif // WIDGETS_H