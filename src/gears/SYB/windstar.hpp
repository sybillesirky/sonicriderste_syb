#pragma once

#include "context.hpp"

struct WindStarInfo {
    u8 trickAccumulator;
    u8 levelHolder;
    u8 tailwindFrames;
    BOOL beenTricking;
    BOOL tailwindActive;
};

extern WindStarInfo PlayerWindStarInfo[8];

global {
void Player_WindStar(Player *player);
}

global void* gpsTexList_Particle;
global void* lbl_001D8F58;
global void func_Particle_Task();