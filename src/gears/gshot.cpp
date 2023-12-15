#include "gshot.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

std::array<GShotInfo, MaxPlayerCount> PlayerGShotInfo;

void Player_GShot_ResetBoostSpeed(Player *player, int inputLevel) {
    player->gearStats[inputLevel].boostSpeed = Gears[ExtremeGear::GShot].levelStats[inputLevel].boostSpeed;
    if (player->characterArchetype == BoostArchetype) {
		player->gearStats[inputLevel].boostSpeed += BoostArchetypeBoostSpeeds[inputLevel];
	}
}

void Player_GShot(Player *player) {
    GShotInfo *GShotInfo = &PlayerGShotInfo[player->index];
    f32 newSpeed;

    // Clear everything at Start Line or Death.
    if (player->state == StartLine || player->state == Death) {
        GShotInfo->chargeFrames = 0;
        GShotInfo->GBoostBonus = 0;
        GShotInfo->GBoostControl = FALSE;
    }

    if (player->movementFlags & 0x1000) { // Charging Jump
        if (GShotInfo->chargeFrames < 1000) {
            GShotInfo->chargeFrames += 1;
        }
    }
    
    if (!(player->movementFlags & boosting)) { // Emergency Exit in case the Boost Control flag doesn't update.
        GShotInfo->GBoostControl = FALSE;
    }

    if (player->state != Cruise) { // Boost retention blocker.
        GShotInfo->GBoostControl = FALSE;
    }
    
    if (GShotInfo->GBoostControl == TRUE) {
        if (GShotInfo->GBoostBonus < 100.0) {
            GShotInfo->GBoostBonus += 0.5;
            player->speed += pSpeed(0.5);
        }
        newSpeed = Gears[ExtremeGear::GShot].levelStats[player->level].boostSpeed + pSpeed(GShotInfo->GBoostBonus);
        player->gearStats[player->level].boostSpeed = newSpeed;
    }

    if (GShotInfo->GBoostBonus != 0 && GShotInfo->GBoostControl == FALSE) {
        Player_GShot_ResetBoostSpeed(player, 0);
        Player_GShot_ResetBoostSpeed(player, 1);
        Player_GShot_ResetBoostSpeed(player, 2);
        GShotInfo->GBoostBonus = 0;
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3D)); // Speed Shoes SFX
    }
}