#include "collectorarchetype.hpp"
#include "handlers/player/initgeardata.hpp"

// lbl_00089654 = Ring collect function.

ASMUsed void Collector_RingSpeedGain(Player *player) {
    if (player->characterArchetype != CollectorArchetype) return;
    player->speed += pSpeed(5);
    if (~player->specialFlags & ringGear) { // SYB: Otherwise these fuckers get 3x Ring pickup.
        player->currentAir += 2000;
    }
}

ASMUsed void Collector_RingBoxSpeedGain(Player *player, Object *object) {
    // This is currently unused.
    if (player->characterArchetype != CollectorArchetype) return;
    if (object->object_type != ItemBox) return;
    if (
    object->item_id == TenRings ||
    object->item_id == TwentyRings ||
    object->item_id == ThirtyRings ||
    object->item_id == FiveRings) {
    player->speed += pSpeed(25);
    }
    else if (object->item_id == RNG) {
        player->speed += pSpeed(10);
    }
}

ASMUsed void ForFun_DieOn100Box(Player *player, Object *object) {
    return; // This code is funny so I don't wanna completely disable it.
    if (object->object_type != ItemBox) return;
    if (
    object->item_id == HundredRings ||
    object->item_id == MaxAir) {
        player->previousState = player->state;
        player->state = Death;
    }
}

void Player_CollectorArchetype(Player *player) {
    // Legacy.
    if (player->characterArchetype != CollectorArchetype) return;

    player->rings = 100;
}