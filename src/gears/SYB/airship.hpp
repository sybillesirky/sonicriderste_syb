#pragma once

#include "context.hpp"

struct AirshipInfo {
	u8 airdashCooldown;
    
};

extern AirshipInfo PlayerAirshipInfo[8];

global {
void Player_Airship(Player *player);
}