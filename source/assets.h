#ifndef ASSETS_H
#define ASSETS_H
#pragma once

#include <SDL2/SDL_ttf.h>

// Font
extern TTF_Font *gFontS;
extern TTF_Font *gFont;
extern TTF_Font *gFontL;

// Colors
extern SDL_Color BG_COLOR;

// Textures
extern SDL_Texture *bgTexture;
extern SDL_Texture *spritesTexture;
extern SDL_Texture *btnTexture;
extern SDL_Texture *cityTexture;
extern SDL_Texture *uiTexture;
extern SDL_Texture *logoTexture;
extern SDL_Texture *menuBgTexture;

// Clips
struct Clips_OpinionIcon
{
    const SDL_Rect HIGH = {0, 0, 8, 8};
    const SDL_Rect MED = {8, 0, 8, 8};
    const SDL_Rect LOW = {16, 0, 8, 8};
};

// Colors
struct GameColors
{
    const SDL_Color WHITE = {255, 255, 255, 255};
    const SDL_Color BLACK = {0, 0, 0};
    const SDL_Color RED = {180, 32, 42, 255};
    const SDL_Color NAVYBLUE = {38, 48, 64, 255};
    const SDL_Color ORANGE = {249, 163, 27};
    const SDL_Color DARKGREEN = {36, 43, 16, 255};
};

// Info between scenes
struct GameInformation
{
    int score = 0;
    std::string testMsg;
};

#endif // ASSETS_H
