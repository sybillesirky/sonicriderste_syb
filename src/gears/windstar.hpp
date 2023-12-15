#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct WindStarInfo {
    u8 tailwindFrames;
    bool tailwindActive;
};
#pragma GCC diagnostic pop

extern std::array<WindStarInfo, MaxPlayerCount> PlayerWindStarInfo;

void Player_WindStar(Player *player);
