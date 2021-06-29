#include "ZRect.h"

// Constructor
ZRect::ZRect(int x, int y, int w, int h) {
    mRect.x = x;
    mRect.y = y;
    mRect.w = w;
    mRect.h = h;
}

// Getters for sides

int ZRect::getLeft() {
    return mRect.x;
}

int ZRect::getRight() {
    return mRect.x + mRect.w;
}

int ZRect::getTop() {
    return mRect.y;
}

int ZRect::getBottom() {
    return mRect.y + mRect.h;
}

// Getters for center and center components

int ZRect::getCenterX() {
    return mRect.x + (mRect.w / 2);
}

int ZRect::getCenterY() {
    return mRect.y + (mRect.h / 2);
}

SDL_Point ZRect::getCenter() {
    return {getCenterX(), getCenterY()};
}

// Getters for corner points

SDL_Point ZRect::getTopLeft() {
    return {getLeft(), getTop()};
}

SDL_Point ZRect::getTopRight() {
    return {getRight(), getTop()};
}

SDL_Point ZRect::getBottomLeft() {
    return {getLeft(), getBottom()};
}

SDL_Point ZRect::getBottomRight() {
    return {getRight(), getBottom()};
}

// Getters for the mRect member
SDL_Rect ZRect::getRect() {
    return mRect;
}

int ZRect::getX() {
    return mRect.x;
}

int ZRect::getY() {
    return mRect.y;
}

int ZRect::getWidth() {
    return mRect.w;
}

int ZRect::getHeight() {
    return mRect.h;
}

// Setters for the mRect member
SDL_Rect ZRect::setRect(SDL_Rect rect) {
    mRect = rect;
}

int ZRect::setX(int x) {
    mRect.x = x;
}

int ZRect::setY(int y) {
    mRect.y = y;
}

int ZRect::setWidth(int w) {
    mRect.w = w;
}

int ZRect::setHeight(int h) {
    mRect.h = h;
}
