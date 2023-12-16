#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct ArchAngelInfo {
    bool isTransformed;
    u32 ringDrainTickRate;
};
#pragma GCC diagnostic pop

extern std::array<ArchAngelInfo, MaxPlayerCount> PlayerArchAngelInfo;

void Player_ArchAngel(Player *player);

ASMDefined void func_Particle_Task();
ASMDefined void* gpsTexList_Particle;
ASMDefined void* lbl_001D8F58;