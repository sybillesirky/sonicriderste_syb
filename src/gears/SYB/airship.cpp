#include "airship.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

AirshipInfo PlayerAirshipInfo[8];

void Player_Airship(Player *player) {

    if (player->extremeGear != Airship) return;

    AirshipInfo *AirshipInfo = &PlayerAirshipInfo[player->index];

    if (AirshipInfo->airdashCooldown != 0) {
        AirshipInfo->airdashCooldown -= 1;
    }

    if (player->input->holdFaceButtons & XButton && AirshipInfo->airdashCooldown == 0) {
        // Air Dash Charging. +1 every frame only when correct playerstate.
        if (player->state == Fall || player->state == Jump || player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp) {
            if (AirshipInfo->airdashCharge < 120) {
                AirshipInfo->airdashCharge += 1;
            }
        }
        // Always set to 0 if not in one of those states to prevent banking.
        else {
            AirshipInfo->airdashCharge = 0;
        }
    }else{
        if (AirshipInfo->airdashCharge == 0) return; // Return if no charge. Saves resources slightly I think/hope?
        if (player->state == Fall || player->state == Jump || player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp) {
            // Mid/Max charge Air Dash behaviours.
            if (AirshipInfo->airdashCharge > 45) { // Middle Charge
                if (AirshipInfo->airdashCooldown == 0 && player->currentAir > player->gearStats[player->level].boostCost * 2) {
                    if (AirshipInfo->airdashCharge >= 90) { // Max Charge
					    player->speed += pSpeed(350);
					}
                    else {
                        player->speed += pSpeed(225);
                    }
                    // Mid/Max shared behaviours.
                    if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3B)); //Dash panel SFX
                    player->currentAir -= player->gearStats[player->level].boostCost * 2;
                    player->verticalSpeed -= 0.2;
                    AirshipInfo->airdashCooldown = 180;
                }
                AirshipInfo->airdashCharge = 0;
            }
            // Min charge Air Dash behaviours.
            else {
                if (AirshipInfo->airdashCooldown == 0 && player->currentAir > player->gearStats[player->level].boostCost * 2) {
                    player->speed += pSpeed(150);
                    player->speed += pSpeed(AirshipInfo->airdashCharge * 2);
                    player->verticalSpeed -= 0.2;
                    if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3B)); //Dash panel SFX
                    player->currentAir -= player->gearStats[player->level].boostCost * 2;
                    AirshipInfo->airdashCooldown = 180;
                }
                AirshipInfo->airdashCharge = 0;
            }
        }
        else {
            AirshipInfo->airdashCharge = 0;
        }
    }

    // if (player->input->toggleFaceButtons & XButton) {
    //    if (player->state == Fall || player->state == Jump || player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp) {
    //        if (AirshipInfo->airdashCooldown == 0 && player->currentAir > player->gearStats[player->level].boostCost * 2) {
    //            player->speed += pSpeed(150);
    //            player->verticalSpeed -= 0.2;
    //            // player->verticalSpeed += 0.5; // Air Dash height bonus. Legacy!
    //            player->currentAir -= player->gearStats[player->level].boostCost * 2;
    //           AirshipInfo->airdashCooldown = 180;
    //            if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3B)); //Dash panel SFX
    //        }
    //    }
    // }

}