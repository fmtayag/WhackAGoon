#ifndef G_DATA_H
#define G_DATA_H
#pragma once

#include <string>
#include <SDL.h>

struct WindowMetadata
{
    const std::string TITLE = "Whack-A-Thief!";
    const int PXSCALE = 8;
    const int WIDTH = 100 * PXSCALE;
    const int HEIGHT = 100 * PXSCALE;
    const int FPS = 60;
    const int TICKS_PF = 1000 / FPS;
    const int OUT_OF_BOUNDS = -32; // Used for determining if an object is out of the window's bounds.

    const int NATIVE_WIDTH = 100;
    const int NATIVE_HEIGHT = 100;
    const int TARG_WIDTH = NATIVE_WIDTH * PXSCALE;
    const int TARG_HEIGHT = NATIVE_HEIGHT * PXSCALE;
};

struct GameColors
{
    const SDL_Color WHITE = {255, 255, 255, 255};
    const SDL_Color BLACK = {0, 0, 0, 255};
    const SDL_Color NEON_TOXICGREEN = {175, 196, 33, 255};
};

struct GameMouse
{
    bool isClicked;
    SDL_Point position;
};

struct GameRules
{
    const int SCORE_PENALTY = 10;
};

enum PosCentering
{
    POSCEN_NONE,
    POSCEN_X,
    POSCEN_Y,
    POSCEN_BOTH
};

enum DPL // Debug Print Levels
{
    DEBUG,
    WARNING,
    ERROR
};

struct Vector2
{
    int x, y;
};

struct CSize
{
    int w, h;
};

#endif // G_DATA_H