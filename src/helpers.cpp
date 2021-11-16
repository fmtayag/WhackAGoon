#include <iostream>
#include <cassert>
#include <SDL.h>
#include <SDL_image.h>
#include "helpers.h"

const bool isPointCollide(SDL_Point point, SDL_Rect box)
{
    int px = point.x;
    int py = point.y;
    int bx = box.x;
    int by = box.y;
    int bw = box.w;
    int bh = box.h;

    if (px <= bx || px >= bx + bw)
        return false;
    else if (py <= by || py >= by + bh)
        return false;

    return true;
}