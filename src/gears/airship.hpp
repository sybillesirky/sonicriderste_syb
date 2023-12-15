#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct AirshipInfo {
	u8 airdashCooldown;
};
#pragma GCC diagnostic pop

extern std::array<AirshipInfo, MaxPlayerCount> PlayerAirshipInfo;

void Player_Airship(Player *player);
