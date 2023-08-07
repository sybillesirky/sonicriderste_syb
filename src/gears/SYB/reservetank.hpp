#pragma once

#include "context.hpp"

struct ReserveTankInfo {
    u8 tankAmount; // This is solely used to maintain the correct level after a death.
};

extern ReserveTankInfo PlayerReserveTankInfo[8];

global {
void Player_ReserveTank(Player *player);
}

global void* gpsTexList_Particle;
global void* lbl_001D8F58;
global void func_Particle_Task();