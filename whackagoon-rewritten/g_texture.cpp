#include <iostream>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "g_texture.h"
#include "g_data.h"

#pragma region GTexture
GTexture::GTexture()
{
    m_texture = NULL;
}
GTexture::~GTexture()
{
    free();
    printf("GTexture::~GTexture() | Deleted.\n");
}
void GTexture::loadFromFile(std::string path)
{
    m_texture = IMG_LoadTexture(gameRenderer, path.c_str());
    printf("GTexture::loadFromFile() | IMG_Error: %s\n", IMG_GetError());
}
void GTexture::draw(SDL_Rect *clip, SDL_Rect *dst)
{
    SDL_RenderCopy(gameRenderer, m_texture, clip, dst);
    // printf("GTexture::draw() | SDL_Error: %s\n", SDL_GetError());
    // printf("GTexture::draw() | IMG_Error: %s\n", IMG_GetError());
}
void GTexture::free()
{
    if (m_texture != NULL)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = NULL;
    }
}
#pragma endregion GTexture