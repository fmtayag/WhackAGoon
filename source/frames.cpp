#include "frames.h"
#include "utils.h"
#include "sprites.h"

std::map<std::string, std::vector<SDL_Rect>> frm_ClipDat;
std::map<std::string, int> frm_AnimDelayDat;

void loadFrameData() {

    // --- spritesheet clips and animation delays ---

    // Resting animation
    frm_ClipDat[Z_ClipID(AS_Resting, HT_None)] = {
        {0, 16, 16, 16}
    };

    // For Goon ------------------------------------------------
    frm_AnimDelayDat[ Z_ClipID(AS_ToAwake, HT_Goon) ] = 50;
    frm_ClipDat[Z_ClipID(AS_ToAwake, HT_Goon)] = {
        {0, 32, 16, 16},
        {16, 32, 16, 16},
        {32, 32, 16, 16},
        {48, 32, 16, 16},
        {64, 32, 16, 16},
        {80, 32, 16, 16},
        {96, 32, 16, 16},
        {112, 32, 16, 16}
    };

    frm_AnimDelayDat[ Z_ClipID(AS_Awake, HT_Goon) ] = 700;
    frm_ClipDat[Z_ClipID(AS_Awake, HT_Goon)] = {
        {0, 48, 16, 16},
        {16, 48, 16, 16},
        {32, 48, 16, 16},
        {48, 48, 16, 16}
    };

    frm_AnimDelayDat[ Z_ClipID(AS_Whacked, HT_Goon) ] = 100;
    frm_ClipDat[Z_ClipID(AS_Whacked, HT_Goon)] = {
        {0, 64, 16, 16},
        {16, 64, 16, 16},
        {32, 64, 16, 16},
        {48, 64, 16, 16}
    };

    frm_AnimDelayDat[ Z_ClipID(AS_ToResting, HT_Goon) ] = 50;
    frm_ClipDat[Z_ClipID(AS_ToResting, HT_Goon)] = {
        {112, 32, 16, 16},
        {96, 32, 16, 16},
        {80, 32, 16, 16},
        {64, 32, 16, 16},
        {48, 32, 16, 16},
        {32, 32, 16, 16},
        {16, 32, 16, 16},
        {0, 32, 16, 16}
    };


    // For Townie ---------------------------------------------
    frm_AnimDelayDat[ Z_ClipID(AS_ToAwake, HT_Townie) ] = 50;
    frm_ClipDat [ Z_ClipID(AS_ToAwake, HT_Townie) ] = {
        {0, 128, 16, 16},
        {16, 128, 16, 16},
        {32, 128, 16, 16},
        {48, 128, 16, 16},
        {64, 128, 16, 16},
        {80, 128, 16, 16},
        {96, 128, 16, 16},
        {112, 128, 16, 16}
    };

    frm_AnimDelayDat[ Z_ClipID(AS_Awake, HT_Townie) ] = 300;
    frm_ClipDat[ Z_ClipID(AS_Awake, HT_Townie) ] = {
        {0, 144, 16, 16},
        {16, 144, 16, 16},
        {32, 144, 16, 16},
        {48, 144, 16, 16},
        {64, 144, 16, 16},
        {80, 144, 16, 16}
    };

    frm_AnimDelayDat[ Z_ClipID(AS_Whacked, HT_Townie) ] = 100;
    frm_ClipDat[ Z_ClipID(AS_Whacked, HT_Townie) ] = {
        {0, 160, 16, 16},
        {16, 160, 16, 16},
        {32, 160, 16, 16},
        {48, 160, 16, 16}
    };

    frm_AnimDelayDat[ Z_ClipID(AS_ToResting, HT_Townie) ] = 50;
    frm_ClipDat [ Z_ClipID(AS_ToResting, HT_Townie) ] = {
        {112, 128, 16, 16},
        {96, 128, 16, 16},
        {80, 128, 16, 16},
        {64, 128, 16, 16},
        {48, 128, 16, 16},
        {32, 128, 16, 16},
        {16, 128, 16, 16},
        {0, 128, 16, 16}
    };


    // For Mayor ---------------------------------------------------
    frm_AnimDelayDat[ Z_ClipID(AS_ToAwake, HT_Mayor) ] = 50;
    frm_ClipDat [ Z_ClipID(AS_ToAwake, HT_Mayor) ] = {
        {0, 80, 16, 16},
        {16, 80, 16, 16},
        {32, 80, 16, 16},
        {48, 80, 16, 16},
        {64, 80, 16, 16},
        {80, 80, 16, 16},
        {96, 80, 16, 16},
        {112, 80, 16, 16}
    };

    frm_AnimDelayDat[ Z_ClipID(AS_Awake, HT_Mayor) ] = 300;
    frm_ClipDat[ Z_ClipID(AS_Awake, HT_Mayor) ] = {
        {0, 96, 16, 16},
        {16, 96, 16, 16},
        {32, 96, 16, 16},
        {48, 96, 16, 16}
    };

    frm_AnimDelayDat[ Z_ClipID(AS_Whacked, HT_Mayor) ] = 100;
    frm_ClipDat[ Z_ClipID(AS_Whacked, HT_Mayor) ] = {
        {0, 112, 16, 16},
        {16, 112, 16, 16},
        {32, 112, 16, 16},
        {48, 112, 16, 16}
    };

    frm_AnimDelayDat[ Z_ClipID(AS_ToResting, HT_Mayor) ] = 50;
    frm_ClipDat [ Z_ClipID(AS_ToResting, HT_Mayor) ] = {
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
