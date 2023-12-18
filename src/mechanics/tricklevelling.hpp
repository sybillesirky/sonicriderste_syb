#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct TrickLevelInfo {
    u8 trickAccumulator;
    bool isTricking;
};
#pragma GCC diagnostic pop

extern std::array<TrickLevelInfo, MaxPlayerCount> PlayerTrickLevelInfo;

void Player_TrickLevelling(Player *player);
void Player_CreateTrickLevelUpParticles(Player *player);

ASMDefined void func_Particle_Task();
ASMDefined void* gpsTexList_Particle;
ASMDefined void* lbl_001D8F58;