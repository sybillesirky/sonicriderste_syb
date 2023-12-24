#include "warpdrive.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

std::array<WarpDriveInfo, MaxPlayerCount> PlayerWarpDriveInfo;

bool Player_WarpDrive_CheckPlayerValidity(Player *player) {
    bool validity = (player->state == Cruise ||
            player->state == Fall ||
            player->state == Jump ||
            player->state == Fly);
    return validity;
}

void Player_WarpDrive(Player *player) {
    
    WarpDriveInfo *WarpDriveInfo = &PlayerWarpDriveInfo[player->index];

    if (player->input->toggleFaceButtons.hasAny(DPadUp) &&
    player->slipstream == true &&
    WarpDriveInfo->isTeleportChargeActive ==  false) {
        WarpDriveInfo->destinationPlayer = WarpDriveInfo->slipstreamPlayer;
        WarpDriveInfo->isTeleportChargeActive = true;
        WarpDriveInfo->countdownTeleportFrames = 180;
        if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3F)); // Battle Mode SFX
    }

    // Count down the timer if necessary
    if (WarpDriveInfo->isTeleportChargeActive == true) {
        WarpDriveInfo->countdownTeleportFrames -= 1;
    }

    // Check teleport validity and teleport once timer reaches zero
    if (WarpDriveInfo->countdownTeleportFrames == 0 && WarpDriveInfo->isTeleportChargeActive == true) {

        // Check validity
        if (!(Player_WarpDrive_CheckPlayerValidity(player)) || !(Player_WarpDrive_CheckPlayerValidity(WarpDriveInfo->destinationPlayer))) {
            WarpDriveInfo->isTeleportChargeActive = false;
            if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x0F)); // Superbad Buzz
        }
        // Teleport
        else {
            // Set new positions
            WarpDriveInfo->PlayerPos = {player->x, player->y, player->z};
            player->x = WarpDriveInfo->destinationPlayer->x;
            player->y = WarpDriveInfo->destinationPlayer->y;
            player->z = WarpDriveInfo->destinationPlayer->z;
            WarpDriveInfo->destinationPlayer->x = WarpDriveInfo->PlayerPos[0];
            WarpDriveInfo->destinationPlayer->y = WarpDriveInfo->PlayerPos[1];
            WarpDriveInfo->destinationPlayer->z = WarpDriveInfo->PlayerPos[2];

            // Set player states if necessary.
            if (player->state == Fly) {
                player->state = Fall;
            }
            if (WarpDriveInfo->destinationPlayer->state == Fly) {
                WarpDriveInfo->destinationPlayer->state = Fall;
            }

            // Induce costs
            player->currentAir -= player->gearStats[player->level].boostCost * 2;
            if (player->currentAir < 0) {
                player->currentAir = 0;
            }

            // SFX + set flag back
            if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x12)); // Exit Pit SFX
            WarpDriveInfo->isTeleportChargeActive = false;
        }
    }
}