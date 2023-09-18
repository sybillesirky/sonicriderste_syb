#pragma once

#include "context.hpp"

struct AngryArnoldInfo {
	u8 CurrentArnoldLevel;
    u8 LatestArnoldLevel; // SYB: Easiest way to handle level up behaviour.
    u16 ArnoldClock; // SYB: Easy frame clock
};

extern AngryArnoldInfo PlayerAngryArnoldInfo[8];

global {
    void Player_AngryArnold(Player *player);
}

global void* gpsTexList_Particle;
global void* lbl_001D8F58;
global void func_Particle_Task();