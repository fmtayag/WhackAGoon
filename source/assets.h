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

// Clips
struct opine_clips
{
    SDL_Rect high = {0, 0, 8, 8};
    SDL_Rect med = {8, 0, 8, 8};
    SDL_Rect low = {16, 0, 8, 8};
};

#endif // ASSETS_H
