#ifndef WIDGETS_H
#define WIDGETS_H
#pragma once

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "g_data.h"

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
    bool loadedAsTarget = false;

public:
    GTexture();
    ~GTexture();

    void loadFromFile(std::string path);
    void loadAsTarget(SDL_Rect size);
    void free();
    void draw(SDL_Rect *clip = NULL, SDL_Rect *dst = NULL);
    int fetchWidth();
    int fetchHeight();

    void setAsTarget();
    void unsetAsTarget();
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

class ShakeGenerator
{
private:
    SDL_Point m_displacement;
    std::vector<int> m_shakevalues;
    int m_shakeIntensity;
    static const int MAX_SHAKE = 5;

    GTimer m_tmrShakeRect;
    Uint32 m_delayShakeRect = 40;

public:
    ShakeGenerator();
    ~ShakeGenerator();

    void update();
    void generateShake();
    SDL_Point fetchDisplacement() { return m_displacement; };
};

#endif // WIDGETS_H