#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct GShotInfo {
	u16 chargeFrames;
	bool GBoostControl;
	f32 GBoostBonus;
};
#pragma GCC diagnostic pop

extern std::array<GShotInfo, MaxPlayerCount> PlayerGShotInfo;

void Player_GShot(Player *player);