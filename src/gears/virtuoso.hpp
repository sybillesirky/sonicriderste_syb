#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct VirtuosoInfo {
	bool performanceMode;
	u16 timeSinceLastNote;
	u16 totalNotesPlayed;
	u16 performanceDuration;
};
#pragma GCC diagnostic pop

extern std::array<VirtuosoInfo, MaxPlayerCount> PlayerVirtuosoInfo;

void Player_Virtuoso(Player *player);
