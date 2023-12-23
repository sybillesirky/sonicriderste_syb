#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct AccumulatorInfo {
	f32 collectedSpeed;
    u8 dashPanelCooldown;
};
#pragma GCC diagnostic pop

extern std::array<AccumulatorInfo, MaxPlayerCount> PlayerAccumulatorInfo;

void Player_Accumulator(Player *player);
