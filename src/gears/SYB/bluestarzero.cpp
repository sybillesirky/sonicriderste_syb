#include "bluestarzero.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

BSZInfo PlayerBSZInfo[8];

void Player_BSZ(Player *player) {
    if (player->extremeGear != BlueStarZero) return;

    BSZInfo *BSZInfo = &PlayerBSZInfo[player->index];

    if (player->input->holdFaceButtons & XButton && player->currentAir > 1) {
        if (player->state == Jump || player->state == Fall) {
            if (BSZInfo->GravDiveStartupTimer != 0) {
                player->speed = player->speed / 1.01;
                BSZInfo->GravDiveStartupTimer -= 1;
                player->verticalSpeed = 0.1;
                return;
            }

            if (BSZInfo->GravDiveState == false) {
                BSZInfo->GravDiveState = true;
                if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3B)); //Dash panel SFX
            }
            
            player->speed = pSpeed(400);
            player->currentAir -= 500;

            if (player->input->holdFaceButtons & LStickDown) {
                BSZInfo->verticalVelocity += 0.02;
                if (BSZInfo->verticalVelocity > 0.5) {
                    BSZInfo->verticalVelocity = 0.5;
                }
            }
            else if (player->input->holdFaceButtons & LStickUp) {
                BSZInfo->verticalVelocity -= 0.02;
                if (BSZInfo->verticalVelocity < -0.5) {
                    BSZInfo->verticalVelocity = -0.5;
                }
            }
            else {
                BSZInfo->verticalVelocity = BSZInfo->verticalVelocity / 1.1;
            }

            player->verticalSpeed = BSZInfo->verticalVelocity;
        }
    }else{
        BSZInfo->verticalVelocity = 0;
        BSZInfo->GravDiveStartupTimer = 60;
        BSZInfo->GravDiveState = false;
    }
}