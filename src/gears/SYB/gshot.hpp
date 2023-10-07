#pragma once

#include "context.hpp"

struct GShotInfo {
	u16 chargeFrames;
	BOOL GBoostControl;
	f32 GBoostBonus;
};

extern GShotInfo PlayerGShotInfo[8];

global {
void Player_GShot(Player *player);
}