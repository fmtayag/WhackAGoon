#ifndef G_TEXTURE_H
#define G_TEXTURE_H
#pragma once

#include <string>
#include <SDL2/SDL.h>

class GTexture
{
private:
    SDL_Texture *m_texture;

public:
    GTexture();
    ~GTexture();

    void loadFromFile(std::string path);
    void free();
    void draw(SDL_Rect *clip = NULL, SDL_Rect *dst = NULL);
    int fetchWidth();
    int fetchHeight();
};

extern GTexture uiTexture;

#endif // G_TEXTURE_H