#pragma once

#include "context.hpp"

global {
void Player_CollectorArchetype(Player *player);
}

ASMUsed void Collector_RingSpeedGain(Player *player);
ASMUsed void Collector_RingBoxSpeedGain(Player *player, Object *object);