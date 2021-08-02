#include <iostream>
#include <map>
#include <tuple>
#include <SDL2/SDL.h>
#include "sprites.h"
#include "utils.h"

HoleSprite::HoleSprite(SDL_Texture* spritesheet, int x, int y) {
    mSpritesheet = spritesheet;
    mRect.x = x;
    mRect.y = y;
    mRect.w = 128;
    mRect.h = 128;

    m_CurFrame = 0;
    m_AnimState = AS_Resting;
    m_Type = HT_None;

    anim_timer = SDL_GetTicks();

    // --- spritesheet clips and animation delays ---

    // Resting animation
    mpClips[Z_ClipID(AS_Resting, HT_None)] = {
        {0, 16, 16, 16}
    };


    // For Goon ---
    mpAnimDelays[ Z_ClipID(AS_ToAwake, HT_Goon) ] = 100;
    mpClips[Z_ClipID(AS_ToAwake, HT_Goon)] = {
        {0, 32, 16, 16},
        {16, 32, 16, 16},
        {32, 32, 16, 16},
        {48, 32, 16, 16},
        {64, 32, 16, 16},
        {80, 32, 16, 16},
        {96, 32, 16, 16},
        {112, 32, 16, 16}
    };

    mpAnimDelays[ Z_ClipID(AS_Awake, HT_Goon) ] = 700;
    mpClips[Z_ClipID(AS_Awake, HT_Goon)] = {
        {0, 48, 16, 16},
        {16, 48, 16, 16},
        {32, 48, 16, 16},
        {48, 48, 16, 16}
    };

    mpAnimDelays[ Z_ClipID(AS_Whacked, HT_Goon) ] = 100;
    mpClips[Z_ClipID(AS_Whacked, HT_Goon)] = {
        {0, 64, 16, 16},
        {16, 64, 16, 16},
        {32, 64, 16, 16},
        {48, 64, 16, 16}
    };

    mpAnimDelays[ Z_ClipID(AS_ToResting, HT_Goon) ] = 100;
    mpClips[Z_ClipID(AS_ToResting, HT_Goon)] = {
        {112, 32, 16, 16},
        {96, 32, 16, 16},
        {80, 32, 16, 16},
        {64, 32, 16, 16},
        {48, 32, 16, 16},
        {32, 32, 16, 16},
        {16, 32, 16, 16},
        {0, 32, 16, 16}
    };


    // For Townie ---
    /** TODO **/


    // For Mayor ---
    /** TODO **/


}

HoleSprite::~HoleSprite() {

}

void HoleSprite::update() {
    animate();

}

void HoleSprite::draw(SDL_Renderer* renderer) {

    SDL_RenderCopy(renderer,
                   mSpritesheet,
                   &mpClips[ Z_ClipID(m_AnimState, m_Type) ][m_CurFrame],
                   &mRect);
}

void HoleSprite::awake(HoleType hType) {
    if(m_AnimState == AS_Resting) {
        m_AnimState = AS_ToAwake;
        m_Type = hType; /** TODO: for debug only. **/
         m_CurFrame = 0;
    }
    else {
        printf("Debug: HoleSprite was awakened but was not in Resting state.\n");
    }
}

void HoleSprite::whack() {
    if(m_AnimState == AS_Awake) {
        m_AnimState = AS_Whacked;
        m_CurFrame = 0;
        whacked_timer = SDL_GetTicks();
    }
    else {
        printf("Debug: HoleSprite was whacked but was not in Awake state.\n");
    }
}

void HoleSprite::animate() {

    if(m_AnimState != AS_Resting) {
        int now = SDL_GetTicks();
        if(now - anim_timer > mpAnimDelays[ Z_ClipID(m_AnimState, m_Type) ]) {
            anim_timer = now;

            if(m_CurFrame < mpClips[ Z_ClipID(m_AnimState, m_Type) ].size() - 1 ) {
                m_CurFrame += 1;
            }
            else {
                m_CurFrame = 0;

                // Switch state check
                if(m_AnimState == AS_ToAwake) {
                    m_AnimState = AS_Awake;
                    awake_timer = SDL_GetTicks();
                    awake_dur = 5000;
                }
                else if(m_AnimState == AS_Awake) {
                    now = SDL_GetTicks();
                    if(now - awake_timer > awake_dur) {
                        m_AnimState = AS_ToResting;
                    }
                }
                else if(m_AnimState == AS_Whacked) {
                    now = SDL_GetTicks();
                    if(now - whacked_timer > WHACKED_DUR) {
                        m_AnimState = AS_ToResting;
                    }
                }
                else if(m_AnimState == AS_ToResting) {
                    m_AnimState = AS_Resting;
                    m_Type = HT_None;
                }
            }
        }
    }

}

