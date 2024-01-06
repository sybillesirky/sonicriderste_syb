#include "collectorarchetype.hpp"
#include "handlers/player/initgeardata.hpp"

// lbl_00089654 = Ring collect function.

ASMUsed void Collector_RingSpeedGain(Player *player) {
    if (player->characterArchetype != Collector) return;

    player->speed += pSpeed(5);
    if (player->extremeGear == ExtremeGear::AdvantageP) {
        player->speed += pSpeed(2.5);
    }
    if (~player->specialFlags & ringGear) { // SYB: Otherwise these fuckers get 3x Ring pickup.
        player->currentAir += 2000;
    }
}