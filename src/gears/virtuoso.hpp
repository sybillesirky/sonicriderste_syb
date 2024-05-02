#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct VirtuosoInfo {
	bool performanceMode;
};
#pragma GCC diagnostic pop

extern std::array<VirtuosoInfo, MaxPlayerCount> PlayerVirtuosoInfo;

void Player_Virtuoso(Player *player);
