#pragma once

#include "context.hpp"

struct ShootingStarInfo {
    u8 trickAccumulator;
    u8 levelHolder;
    BOOL beenTricking;
};

extern ShootingStarInfo PlayerShootingStarInfo[8];

global {
    void Player_ShootingStar(Player *player);
}

global void* gpsTexList_Particle;
global void* lbl_001D8F58;
global void func_Particle_Task();