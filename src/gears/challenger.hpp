#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

namespace Challenger{
	enum GearSpecific{
		Level4,
	};
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct ChallengerInfo {
    u32 timerFrames;
};
#pragma GCC diagnostic pop

extern std::array<ChallengerInfo, MaxPlayerCount> PlayerChallengerInfo;

void Player_Challenger(Player *player);
void Player_CreateChallengerParticles(Player *player);

ASMDefined void func_Particle_Task();
ASMDefined void* gpsTexList_Particle;
ASMDefined void* lbl_001D8F58;
