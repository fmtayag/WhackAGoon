#ifndef WIDGETS_H
#define WIDGETS_H
#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "g_data.h"

enum PosCentering; // MSBuild generates CE CS2061 if this isn't in place.

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

class GFont
{
private:
    TTF_Font *m_font;

public:
    GFont();
    ~GFont();

    void loadFontFromFile(std::string path, int fontSize);
    void free();
    void draw(std::string msg, SDL_Point pos, SDL_Color clr, PosCentering poscenter);
    void drawWithAlpha(std::string msg, SDL_Point pos, SDL_Color clr, PosCentering poscenter, Uint8 alpha);
};

#endif // WIDGETS_H