#pragma once

#include "context.hpp"

struct GShotInfo {
	u16 chargeFrames;
};

extern GShotInfo PlayerGShotInfo[8];

global {
void Player_GShot(Player *player);
}