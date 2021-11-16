#ifndef G_DATA_H
#define G_DATA_H
#pragma once

#include <string>
#include <SDL.h>
#include "widgets.h"

extern SDL_Window *gameWindow;
extern SDL_Renderer *gameRenderer;

struct WindowMetadata
{
    const std::string TITLE = "Whack-A-Goon!";
    const int PXSCALE = 8;
    const int WIDTH = 100 * PXSCALE;
    const int HEIGHT = 100 * PXSCALE;
    const int FPS = 60;
};

struct GameColors
{
    const SDL_Color WHITE = {255, 255, 255, 255};
    const SDL_Color BLACK = {0, 0, 0, 255};
};

struct GameMouse
{
    bool isClicked;
    SDL_Point position;
};

enum PosCentering
{
    POSCEN_NONE,
    POSCEN_X,
    POSCEN_Y,
    POSCEN_BOTH
};

#endif // G_DATA_H