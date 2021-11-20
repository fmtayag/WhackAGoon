#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "widgets.h"
#include "g_data.h"
#include "sprites.h"
#include "helpers.h"

#pragma region GTimer
GTimer::GTimer()
{
    m_startTicks = 0;
    m_pausedTicks = 0;
    m_state = GTS_STOPPED;
}
void GTimer::start()
{
    m_state = GTS_STARTED;
    m_startTicks = SDL_GetTicks();
    m_pausedTicks = 0;
}
void GTimer::stop()
{
    m_state = GTS_STOPPED;
    m_startTicks = 0;
    m_pausedTicks = 0;
}
void GTimer::pause()
{
    if (m_state == GTS_STARTED)
    {
        m_state = GTS_PAUSED;

        m_pausedTicks = SDL_GetTicks() - m_startTicks;
        m_startTicks = 0;
    }
}
void GTimer::unpause()
{
    if (m_state == GTS_PAUSED)
    {
        m_state = GTS_STARTED;
        m_startTicks = SDL_GetTicks() - m_pausedTicks;
        m_pausedTicks = 0;
    }
}
Uint32 GTimer::getTicks()
{
    Uint32 time = 0;
    if (m_state == GTS_STARTED)
        time = SDL_GetTicks() - m_startTicks;
    else if (m_state == GTS_PAUSED)
        time = m_pausedTicks;

    return time;
}
#pragma endregion GTimer

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
    if (m_texture != NULL)
        SDL_RenderCopy(gameRenderer, m_texture, clip, dst);
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

#pragma region GFont
GFont::GFont()
{
    m_font = NULL;
}
GFont::~GFont()
{
    free();
}
void GFont::loadFontFromFile(std::string path, int fontSize)
{
    m_font = TTF_OpenFont(path.c_str(), fontSize);
}
void GFont::free()
{
    if (m_font != NULL)
    {
        TTF_CloseFont(m_font);
        m_font = NULL;
    }
}
void GFont::draw(std::string msg, SDL_Point pos, SDL_Color clr, PosCentering poscenter)
{
    if (m_font != NULL)
    {
        // Create temp surface
        SDL_Surface *msgSurf = TTF_RenderText_Solid(m_font, msg.c_str(), clr);

        // Get width and height
        const int w = msgSurf->w;
        const int h = msgSurf->h;
        SDL_Rect size = {0, 0, w, h};

        // Center position if applicable
        centerPos(pos, size, poscenter);

        // Create rect
        SDL_Rect msgRect = {pos.x, pos.y, size.w, size.h};

        // Create texture
        SDL_Texture *msgTexture = SDL_CreateTextureFromSurface(gameRenderer, msgSurf);

        // Draw texture
        SDL_RenderCopy(gameRenderer, msgTexture, NULL, &msgRect);

        // Free texture and surface
        SDL_FreeSurface(msgSurf);
        SDL_DestroyTexture(msgTexture);
    }
}
void GFont::drawWithAlpha(std::string msg, SDL_Point pos, SDL_Color clr, PosCentering poscenter, Uint8 alpha)
{
    if (m_font != NULL)
    {
        // Create temp surface
        SDL_Surface *msgSurf = TTF_RenderText_Solid(m_font, msg.c_str(), clr);

        // Get width and height
        const int w = msgSurf->w;
        const int h = msgSurf->h;
        SDL_Rect size = {0, 0, w, h};

        // Center position if applicable
        centerPos(pos, size, poscenter);

        // Create rect
        SDL_Rect msgRect = {pos.x, pos.y, size.w, size.h};

        // Create texture
        SDL_Texture *msgTexture = SDL_CreateTextureFromSurface(gameRenderer, msgSurf);

        // Set Blend Mode
        SDL_SetTextureBlendMode(msgTexture, SDL_BLENDMODE_BLEND);

        // Set Alpha
        SDL_SetTextureAlphaMod(msgTexture, alpha);

        // Draw texture
        SDL_RenderCopy(gameRenderer, msgTexture, NULL, &msgRect);

        // Free texture and surface
        SDL_FreeSurface(msgSurf);
        SDL_DestroyTexture(msgTexture);
    }
}
#pragma endregion GFont
