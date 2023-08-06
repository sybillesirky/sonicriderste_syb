#pragma once

#include "context.hpp"

struct RoadStarInfo {
    u8 trickAccumulator;
    u8 levelHolder;
    BOOL beenTricking;
};

extern RoadStarInfo PlayerRoadStarInfo[8];

global {
void Player_RoadStar(Player *player);
}

global void* gpsTexList_Particle;
global void* lbl_001D8F58;
global void func_Particle_Task();