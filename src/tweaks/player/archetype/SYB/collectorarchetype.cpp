#include "collectorarchetype.hpp"
#include "handlers/player/initgeardata.hpp"

// lbl_00089654 = Ring collect function.

ASMUsed void Collector_RingSpeedGain(Player *player) {
    if (player->characterArchetype != CollectorArchetype) return;
    player->speed += pSpeed(5);
    player->currentAir += 2000;
}

ASMUsed void Collector_RingBoxSpeedGain(Player *player, Object *object) {
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

void Player_CollectorArchetype(Player *player) {
    if (player->characterArchetype != CollectorArchetype) return;

    player->rings = 100;
}