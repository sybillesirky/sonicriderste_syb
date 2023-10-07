#include "gshot.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

GShotInfo PlayerGShotInfo[8];

constexpr s16 GShot_DefaultBoostSpeeds[3] = {200, 230, 250}; // for each level

void Player_GShot_ResetBoostSpeed(Player *player, int inputLevel) {
    player->gearStats[inputLevel].boostSpeed = pSpeed(GShot_DefaultBoostSpeeds[inputLevel]);
    if (player->characterArchetype == BoostArchetype) {
		player->gearStats[inputLevel].boostSpeed += BoostArchetypeBoostSpeeds[inputLevel];
	}
}

void Player_GShot(Player *player) {
    if (player->extremeGear != GShot) return;

    GShotInfo *GShotInfo = &PlayerGShotInfo[player->index];

    if (player->movementFlags & 0x1000) { // Charging Jump
        GShotInfo->chargeFrames += 1;
    }

    if (!(player->movementFlags & boosting)) { // Emergency Exit in case the Boost Control flag doesn't update.
        GShotInfo->GBoostControl = FALSE;
    }

    if (player->state != Cruise) { // Boost retention blocker.
        GShotInfo->GBoostControl = FALSE;
    }

    if (GShotInfo->GBoostControl == TRUE) {
        if (GShotInfo->GBoostBonus < 100) {
            GShotInfo->GBoostBonus += 0.05;
        }
        player->gearStats[player->level].boostSpeed = player->gearStats[player->level].boostSpeed + pSpeed(GShotInfo->GBoostBonus);
    }

    if (GShotInfo->GBoostBonus != 0 && GShotInfo->GBoostControl == FALSE) {
        Player_GShot_ResetBoostSpeed(player, 0);
        Player_GShot_ResetBoostSpeed(player, 1);
        Player_GShot_ResetBoostSpeed(player, 2);
        GShotInfo->GBoostBonus = 0;
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3D)); // Speed Shoes SFX
    }
}