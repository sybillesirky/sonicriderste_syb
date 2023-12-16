#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

namespace Wanted{
	enum GearSpecific{
		Level4,

	};
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct WantedInfo {
	u8 ringTimer;
    u8 wantedLevel;
    u8 lastWantedLevel;
    u8 storedPlacement;
};
#pragma GCC diagnostic pop

extern std::array<WantedInfo, MaxPlayerCount> PlayerWantedInfo;

void Player_Wanted(Player *player);

ASMDefined void func_Particle_Task();
ASMDefined void* gpsTexList_Particle;
ASMDefined void* lbl_001D8F58;