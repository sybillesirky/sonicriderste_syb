#pragma once

#include "context.hpp"

struct ArchAngelInfo {
    BOOL isTransformed;
    u32 ringDrainTickRate;
};

extern ArchAngelInfo PlayerArchAngelInfo[8];

global {
void Player_ArchAngel(Player *player);
}

global void* gpsTexList_Particle;
global void* lbl_001D8F58;
global void func_Particle_Task();