#pragma once

#include "context.hpp"

struct BSZInfo {
	f32 verticalVelocity;
    BOOL GravDiveState;
    u16 GravDiveStartupTimer;
};

extern BSZInfo PlayerBSZInfo[8];

global {
void Player_BSZ(Player *player);
}