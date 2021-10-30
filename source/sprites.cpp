#include <iostream>
#include <map>
#include <tuple>
#include <SDL2/SDL.h>
#include "sprites.h"
#include "utils.h"
#include "frames.h"
#include "metadata.h"
#include "assets.h"

#pragma region HoleSprite
//{ HoleSprite
HoleSprite::HoleSprite(SDL_Texture *spritesheet, int x, int y)
{
    mSpritesheet = spritesheet;
    m_Rect.x = x;
    m_Rect.y = y;
    m_Rect.w = HOLE_WIDTH;
    m_Rect.h = HOLE_HEIGHT;

    m_CurFrame = 0;
    m_AnimState = AS_Resting;
    m_Type = HT_None;

    anim_timer = SDL_GetTicks();
}

void HoleSprite::update()
{
    if (m_AnimState != AS_Resting)
    {
        animate();
    }
}

void HoleSprite::draw(SDL_Renderer *renderer)
{

    SDL_RenderCopy(renderer,
                   mSpritesheet,
                   &frm_ClipDat[Z_ClipID(m_AnimState, m_Type)][m_CurFrame],
                   &m_Rect);
}

void HoleSprite::awake(HoleType hType)
{
    if (m_AnimState == AS_Resting)
    {
        m_AnimState = AS_ToAwake;
        m_Type = hType;
        m_CurFrame = 0;
        awake_dur = rand() % 2500 + 800;
        awake_timer = SDL_GetTicks();
        //        printf("Debug: Awake duration is %d ticks.\n", awake_dur);
    }
    else
    {
        //        printf("Debug: HoleSprite was awakened but was not in Resting state.\n");
    }
}

bool HoleSprite::whack()
{
    if (m_AnimState == AS_Awake || m_AnimState == AS_ToAwake)
    {
        m_AnimState = AS_Whacked;
        m_CurFrame = 0;
        whacked_timer = SDL_GetTicks();
        return true;
    }
    else
    {
        //        printf("Debug: HoleSprite was whacked but was not in Awake nor ToAwake state.\n");
        return false;
    }
}

void HoleSprite::animate()
{

    std::string frmID = Z_ClipID(m_AnimState, m_Type);
    const int FRM_DELAY = frm_AnimDelayDat[frmID];
    const int FRM_MAXNO = frm_ClipDat[frmID].size() - 1;

    int now = SDL_GetTicks();

    if (now - anim_timer > FRM_DELAY)
    {
        anim_timer = now;

        if (m_CurFrame < FRM_MAXNO)
        {
            m_CurFrame += 1;
        }
        else
        {
            m_CurFrame = 0;

            // these state transitions are dependent on the animation finishing...
            if (m_AnimState == AS_ToAwake)
            {
                m_AnimState = AS_Awake;
            }
            else if (m_AnimState == AS_ToResting)
            {
                m_AnimState = AS_Resting;
                m_Type = HT_None;
            }
        }
    }

    // ... and these are not
    if (m_AnimState == AS_Awake)
    {
        if (now - awake_timer > awake_dur)
        {
            m_AnimState = AS_ToResting;
            m_CurFrame = 0;
        }
    }
    else if (m_AnimState == AS_Whacked)
    {
        if (now - whacked_timer > WHACKED_DUR)
        {
            m_AnimState = AS_ToResting;
            m_CurFrame = 0;
        }
    }
}
//}
#pragma endregion HoleSprite

#pragma region Particle
//{ Particle
Particle::Particle(SDL_Rect rect, SDL_Color color, SDL_Point velocity)
{
    m_rect = rect;
    m_color = color;
    m_velocity = velocity;
}
Particle::~Particle()
{
    printf("Debug: Particle deleted.\n");
}
void Particle::update()
{
    m_rect.x += m_velocity.x;
    m_rect.y += m_velocity.y;
}
void Particle::draw(SDL_Renderer *renderer)
{
    SDL_Texture *targTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        m_rect.w,
        m_rect.h);
    SDL_SetRenderTarget(renderer, targTexture);
    SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderClear(renderer);
    SDL_RenderFillRect(renderer, NULL);
    SDL_SetRenderTarget(renderer, m_parentTargTexture);
    SDL_RenderCopy(renderer, targTexture, NULL, &m_rect);

    SDL_DestroyTexture(targTexture);
    //printf("SDL_Error: %s\n", SDL_GetError());
}

bool Particle::isVisible()
{
    if ((m_rect.x < -m_rect.w || m_rect.x > WINDOW_WIDTH + m_rect.w) ||
        (m_rect.y < -m_rect.h || m_rect.y > WINDOW_HEIGHT + m_rect.h))
    {
        return false;
    }
    return true;
}

//}
#pragma endregion Particle

#pragma region UI_Icon
//{ UI_Icon
UI_Icon::UI_Icon(SDL_Texture *texture, SDL_Rect rect, int bob_delay)
{
    m_texture = texture;
    m_rect = rect;
    delay_bobbing = bob_delay;
    tmr_bobbing = SDL_GetTicks();
    m_offsety = 0;
    m_origy = m_rect.y;
}
UI_Icon::~UI_Icon()
{
}
void UI_Icon::update()
{
    int now = SDL_GetTicks();
    if (now - tmr_bobbing > delay_bobbing)
    {
        tmr_bobbing = now;
        m_offsety ^= 1;
        m_rect.y = m_origy + (2 * m_offsety);
        printf("m_offsety = %d\n", m_offsety);
    }
}
void UI_Icon::draw(SDL_Renderer *renderer)
{
    //printf("Drawing icon.\n");
    SDL_RenderCopy(renderer, m_texture, &m_clip, &m_rect);
}
void UI_Icon::set_clip(SDL_Rect clip)
{
    m_clip = clip;
}
//}
#pragma endregion UI_Icon