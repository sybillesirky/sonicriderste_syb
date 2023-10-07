#include "gshot.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

GShotInfo PlayerGShotInfo[8];

void Player_GShot(Player *player) {

    if (player->extremeGear != GShot) return;

    GShotInfo *GShotInfo = &PlayerGShotInfo[player->index];

    if (player->movementFlags & 0x1000) { // Charging Jump
        GShotInfo->chargeFrames += 1;
    }
}