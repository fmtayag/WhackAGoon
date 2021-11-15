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

class GTexture
{
private:
    SDL_Texture *m_texture;

public:
    GTexture();
    ~GTexture();

    void loadFromFile(std::string path);
    void free();
    void draw(SDL_Rect *clip = NULL, SDL_Rect *dst = NULL);
    int fetchWidth();
    int fetchHeight();
};

#endif // WIDGETS_H