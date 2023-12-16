#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct WarpDriveInfo {
    Player *slipstreamPlayer = nullptr; // Who's slipstream you are in. We get this from the slipstream function just like GR.
    Player *destinationPlayer = nullptr; // The teleport destination, separate so we can hold it.
    bool isTeleportChargeActive;
    u8 countdownTeleportFrames;
    std::array<f32, 3> PlayerPos;
};
#pragma GCC diagnostic pop

extern std::array<WarpDriveInfo, MaxPlayerCount> PlayerWarpDriveInfo;

void Player_WarpDrive(Player *player);
