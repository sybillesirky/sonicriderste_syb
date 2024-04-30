#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct ProjektRedInfo {
	bool isDead;
    bool redeployState;
    u8 ringTimer;
};
#pragma GCC diagnostic pop

extern std::array<ProjektRedInfo, MaxPlayerCount> PlayerProjektRedInfo;

void Player_ProjektRed(Player *player);
