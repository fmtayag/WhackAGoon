#include <iostream>
#include <map>
#include <tuple>
#include <SDL2/SDL.h>
#include "sprites.h"
#include "utils.h"
#include "frames.h"
#include "metadata.h"

HoleSprite::HoleSprite(SDL_Texture* spritesheet, int x, int y) {
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

HoleSprite::~HoleSprite() {

}

void HoleSprite::update() {
    if(m_AnimState != AS_Resting) {
        animate();
    }

}

void HoleSprite::draw(SDL_Renderer* renderer) {

    SDL_RenderCopy(renderer,
                   mSpritesheet,
                   &frm_ClipDat[ Z_ClipID(m_AnimState, m_Type) ][m_CurFrame],
                   &m_Rect);
}

void HoleSprite::awake(HoleType hType) {
    if(m_AnimState == AS_Resting) {
        m_AnimState = AS_ToAwake;
        m_Type = hType;
        m_CurFrame = 0;
        awake_dur = rand() % 2500 + 800;
        awake_timer = SDL_GetTicks();
//        printf("Debug: Awake duration is %d ticks.\n", awake_dur);
    }
    else {
//        printf("Debug: HoleSprite was awakened but was not in Resting state.\n");
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
//        printf("Debug: HoleSprite was whacked but was not in Awake nor ToAwake state.\n");
        return false;
    }
}

void HoleSprite::animate() {

    std::string frmID = Z_ClipID(m_AnimState, m_Type);
    const int FRM_DELAY = frm_AnimDelayDat[frmID];
    const int FRM_MAXNO = frm_ClipDat[frmID].size() - 1;

    int now = SDL_GetTicks();

    if(now - anim_timer > FRM_DELAY) {
        anim_timer = now;

        if(m_CurFrame < FRM_MAXNO) {
            m_CurFrame += 1;
        }
        else {
            m_CurFrame = 0;

            // these state transitions are dependent on the animation finishing...
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
            m_CurFrame = 0;
        }
    }
    else if(m_AnimState == AS_Whacked) {
        if(now - whacked_timer > WHACKED_DUR) {
            m_AnimState = AS_ToResting;
            m_CurFrame = 0;
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
//                printf("Debug: A mayor is active.\n");
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
                wakeUpDelay = rand() % 1000 + 100;
                break;
            }
        }
    }

}

