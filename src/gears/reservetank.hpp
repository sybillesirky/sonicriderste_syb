#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

namespace ReserveTank{
	enum GearSpecific{
		Level4,
	};
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct ReserveTankInfo {
    u8 tankAmount;
    bool refillActive;
};
#pragma GCC diagnostic pop

extern std::array<ReserveTankInfo, MaxPlayerCount> PlayerReserveTankInfo;

void Player_ReserveTank(Player *player);

ASMDefined void func_Particle_Task();
ASMDefined void* gpsTexList_Particle;
ASMDefined void* lbl_001D8F58;
