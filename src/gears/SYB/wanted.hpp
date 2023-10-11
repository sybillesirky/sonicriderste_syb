#pragma once

#include "context.hpp"

struct WantedInfo {
	u8 ringTimer;
    u8 wantedLevel;
    u8 lastWantedLevel;
    u8 storedPlacement;
};

extern WantedInfo PlayerWantedInfo[8];

global {
void Player_Wanted(Player *player);
}

global void* gpsTexList_Particle;
global void* lbl_001D8F58;
global void func_Particle_Task();