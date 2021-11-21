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
void centerPos(SDL_Point &pos, SDL_Rect &size, PosCentering poscenter)
{
    switch (poscenter)
    {
    case PosCentering::POSCEN_NONE:
        // Do nothing
        break;
    case PosCentering::POSCEN_X:
        pos.x = pos.x - (size.w / 2);
        break;
    case PosCentering::POSCEN_Y:
        pos.y = pos.y - (size.h / 2);
        break;
    case PosCentering::POSCEN_BOTH:
        pos.x = pos.x - (size.w / 2);
        pos.y = pos.y - (size.h / 2);
        break;
    default:
        break;
    }
}
void dbgPrint(DPL dblevel, std::string msg)
{
    std::string finalMsg;

    switch (dblevel)
    {
    case DPL::DEBUG:
        finalMsg.append("[DEBUG]: ");
        break;
    case DPL::WARNING:
        finalMsg.append("[WARNING]: ");
        break;
    case DPL::ERROR:
        finalMsg.append("[ERROR]: ");
        break;
    }

    finalMsg.append(msg);
    finalMsg.append("\n");

    printf(finalMsg.c_str());
}