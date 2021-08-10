#include <iostream>
#include <map>
#include <tuple>
#include <SDL2/SDL.h>
#include "sprites.h"
#include "utils.h"

HoleSprite::HoleSprite(SDL_Texture* spritesheet, int x, int y) {
    mSpritesheet = spritesheet;
    m_Rect.x = x;
    m_Rect.y = y;
    m_Rect.w = 128;
    m_Rect.h = 128;

    m_CurFrame = 0;
    m_AnimState = AS_Resting;
    m_Type = HT_None;

    anim_timer = SDL_GetTicks();

    // --- spritesheet clips and animation delays ---

    // Resting animation
    frames_Clips[Z_ClipID(AS_Resting, HT_None)] = {
        {0, 16, 16, 16}
    };

    // For Goon ------------
    frames_AnimDelays[ Z_ClipID(AS_ToAwake, HT_Goon) ] = 70;
    frames_Clips[Z_ClipID(AS_ToAwake, HT_Goon)] = {
        {0, 32, 16, 16},
        {16, 32, 16, 16},
        {32, 32, 16, 16},
        {48, 32, 16, 16},
        {64, 32, 16, 16},
        {80, 32, 16, 16},
        {96, 32, 16, 16},
        {112, 32, 16, 16}
    };

    frames_AnimDelays[ Z_ClipID(AS_Awake, HT_Goon) ] = 700;
    frames_Clips[Z_ClipID(AS_Awake, HT_Goon)] = {
        {0, 48, 16, 16},
        {16, 48, 16, 16},
        {32, 48, 16, 16},
        {48, 48, 16, 16}
    };

    frames_AnimDelays[ Z_ClipID(AS_Whacked, HT_Goon) ] = 100;
    frames_Clips[Z_ClipID(AS_Whacked, HT_Goon)] = {
        {0, 64, 16, 16},
        {16, 64, 16, 16},
        {32, 64, 16, 16},
        {48, 64, 16, 16}
    };

    frames_AnimDelays[ Z_ClipID(AS_ToResting, HT_Goon) ] = 100;
    frames_Clips[Z_ClipID(AS_ToResting, HT_Goon)] = {
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
    frames_AnimDelays[ Z_ClipID(AS_ToAwake, HT_Townie) ] = 100;
    frames_Clips [ Z_ClipID(AS_ToAwake, HT_Townie) ] = {
        {0, 128, 16, 16}
    };

    frames_AnimDelays[ Z_ClipID(AS_Awake, HT_Townie) ] = 300;
    frames_Clips[ Z_ClipID(AS_Awake, HT_Townie) ] = {
        {0, 144, 16, 16}
    };

    frames_AnimDelays[ Z_ClipID(AS_Whacked, HT_Townie) ] = 100;
    frames_Clips[ Z_ClipID(AS_Whacked, HT_Townie) ] = {
        {0, 160, 16, 16}
    };

    frames_AnimDelays[ Z_ClipID(AS_ToResting, HT_Townie) ] = 100;
    frames_Clips [ Z_ClipID(AS_ToResting, HT_Townie) ] = {
        {0, 128, 16, 16}
    };


    // For Mayor ---
    frames_AnimDelays[ Z_ClipID(AS_ToAwake, HT_Mayor) ] = 100;
    frames_Clips [ Z_ClipID(AS_ToAwake, HT_Mayor) ] = {
        {0, 80, 16, 16},
        {16, 80, 16, 16},
        {32, 80, 16, 16},
        {48, 80, 16, 16},
        {64, 80, 16, 16},
        {80, 80, 16, 16},
        {96, 80, 16, 16},
        {112, 80, 16, 16}
    };

    frames_AnimDelays[ Z_ClipID(AS_Awake, HT_Mayor) ] = 300;
    frames_Clips[ Z_ClipID(AS_Awake, HT_Mayor) ] = {
        {0, 96, 16, 16},
        {16, 96, 16, 16},
        {32, 96, 16, 16},
        {48, 96, 16, 16}
    };

    frames_AnimDelays[ Z_ClipID(AS_Whacked, HT_Mayor) ] = 100;
    frames_Clips[ Z_ClipID(AS_Whacked, HT_Mayor) ] = {
        {0, 112, 16, 16},
        {16, 112, 16, 16},
        {32, 112, 16, 16},
        {48, 112, 16, 16}
    };

    frames_AnimDelays[ Z_ClipID(AS_ToResting, HT_Mayor) ] = 100;
    frames_Clips [ Z_ClipID(AS_ToResting, HT_Mayor) ] = {
        {112, 80, 16, 16},
        {96, 80, 16, 16},
        {80, 80, 16, 16},
        {64, 80, 16, 16},
        {48, 80, 16, 16},
        {32, 80, 16, 16},
        {16, 80, 16, 16},
        {0, 80, 16, 16}
    };

}

HoleSprite::~HoleSprite() {

}

void HoleSprite::update() {
    animate();

}

void HoleSprite::draw(SDL_Renderer* renderer) {

    SDL_RenderCopy(renderer,
                   mSpritesheet,
                   &frames_Clips[ Z_ClipID(m_AnimState, m_Type) ][m_CurFrame],
                   &m_Rect);
}

void HoleSprite::awake(HoleType hType) {
    if(m_AnimState == AS_Resting) {
        m_AnimState = AS_ToAwake;
        m_Type = hType; /** TODO: for debug only. **/
        m_CurFrame = 0;
        awake_dur = rand() % 2500 + 800;
        awake_timer = SDL_GetTicks();
        printf("Debug: Awake duration is %d ticks.\n", awake_dur);
    }
    else {
        printf("Debug: HoleSprite was awakened but was not in Resting state.\n");
    }
}

bool HoleSprite::whack() {
    if(m_AnimState == AS_Awake || m_AnimState == AS_ToAwake) {
        m_AnimState = AS_Whacked;
        m_CurFrame = 0;
        whacked_timer = SDL_GetTicks();
        return true;
    }
    else {
        printf("Debug: HoleSprite was whacked but was not in Awake nor ToAwake state.\n");
        return false;
    }
}

void HoleSprite::animate() {

    if(m_AnimState != AS_Resting) {
        int now = SDL_GetTicks();
        if(now - anim_timer > frames_AnimDelays[ Z_ClipID(m_AnimState, m_Type) ]) {
            anim_timer = now;

            if(m_CurFrame < frames_Clips[ Z_ClipID(m_AnimState, m_Type) ].size() - 1 ) {
                m_CurFrame += 1;
            }
            else {
                m_CurFrame = 0;

                // these state transition checks are dependent on the animation...
                if(m_AnimState == AS_ToAwake) {
                    m_AnimState = AS_Awake;
                }
                else if(m_AnimState == AS_ToResting) {
                    m_AnimState = AS_Resting;
                    m_Type = HT_None;
                }
            }
        }

        // ... and these are not
        if(m_AnimState == AS_Awake) {
            if(now - awake_timer > awake_dur) {
                m_AnimState = AS_ToResting;
            }
        }
        else if(m_AnimState == AS_Whacked) {
            if(now - whacked_timer > WHACKED_DUR) {
                m_AnimState = AS_ToResting;
            }
        }


    }

}














HoleManager::HoleManager(std::vector<HoleSprite*>& holes) {
    m_Holes = &holes;
    wakeUpDelay = 1000;
    wakeUpTimer = SDL_GetTicks();
}

HoleManager::~HoleManager() {
    m_Holes = NULL;
}

void HoleManager::update() {
    using vConstIter = std::vector<HoleSprite*>::const_iterator;

    int now = SDL_GetTicks();
    if(now - wakeUpTimer > wakeUpDelay) {
        wakeUpTimer = now;

        // Check if there is a mayor
        bool mayActv = false;
        for(vConstIter iter = (*m_Holes).begin(); iter != (*m_Holes).end(); iter++ ) {
            if((*iter)->getType() == HT_Mayor) {
                mayActv = true;
                printf("Debug: A mayor is active.\n");
                break;
            }
        }

        // Awake a random hole
        for(vConstIter iter = (*m_Holes).begin(); iter != (*m_Holes).end(); iter++) {
            int selectThis = rand() & 1; // choose either 0 or 1
            if((*iter)->getType() == HT_None && selectThis) {

                int spawnChoice = rand() % (HT_MaxNum-1) + HT_Goon;
                if(mayActv && spawnChoice == (int) HT_Mayor) {
                    spawnChoice = HT_Goon;
                }

                (*iter)->awake((HoleType) spawnChoice);
                wakeUpDelay = rand() % 1500 + 500;
                break;
            }
        }
    }

}

