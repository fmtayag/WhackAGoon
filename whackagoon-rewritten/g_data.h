#ifndef G_DATA_H
#define G_DATA_H
#pragma once

struct GameAssets
{
    SDL_Texture *brickBGTexture;
    SDL_Texture *cityBGTexture;
};

struct WindowMetadata
{
    const std::string TITLE = "Whack-A-Goon!";
    const int PXSCALE = 8;
    const int WINDOW_WIDTH = 100 * PXSCALE;
    const int WINDOW_HEIGHT = 100 * PXSCALE;
    const int FPS = 60;
};

#endif // G_DATA_H