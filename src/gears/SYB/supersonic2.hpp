#pragma once

#include "context.hpp"

struct SuperSonic2Info {
    u8 trickAccumulator;
    u8 levelHolder;
    BOOL beenTricking;
};

extern SuperSonic2Info PlayerSuperSonic2Info[8];

global {
    void Player_SuperSonic2(Player *player);
}

global void* gpsTexList_Particle;
global void* lbl_001D8F58;
global void func_Particle_Task();