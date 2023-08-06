#pragma once

#include "context.hpp"

struct DarkShootingStarInfo {
	u32 P2WFrames; // amount of frames P2W is active for.
    u8 trickAccumulator;
    u8 levelHolder;
    BOOL beenTricking;
    BOOL P2WActive;
};

extern DarkShootingStarInfo PlayerDarkShootingStarInfo[8];

global {
    void Player_DarkShootingStar(Player *player);
}

global void* gpsTexList_Particle;
global void* lbl_001D8F58;
global void func_Particle_Task();