#ifndef ZRECT_H
#define ZRECT_H
#pragma once

#include <SDL2/SDL.h>

/*
 * The ZRect class is just an SDL_Rect with the capability
 * of being able to get the sides, center, and corners of an SDL_Rect.
 */
class ZRect {
public:
    // Constructor
    ZRect(int x, int y, int w, int h);

    // Getters for sides
    int getLeft();
    int getRight();
    int getTop();
    int getBottom();

    // Getters for center and center components
    int getCenterX();
    int getCenterY();
    SDL_Point getCenter();

    // Getters for corner points
    SDL_Point getTopLeft();
    SDL_Point getTopRight();
    SDL_Point getBottomLeft();
    SDL_Point getBottomRight();

    // Getters for the mRect member
    SDL_Rect getRect();
    int getX();
    int getY();
    int getWidth();
    int getHeight();

    // Setters for the mRect member
    SDL_Rect setRect(SDL_Rect rect);
    int setX(int x);
    int setY(int y);
    int setWidth(int w);
    int setHeight(int h);

private:
    SDL_Rect mRect;
};


#endif // ZRECT_H
