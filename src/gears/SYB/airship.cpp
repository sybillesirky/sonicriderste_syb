#include "airship.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

AirshipInfo PlayerAirshipInfo[8];

void Player_Airship(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

    if (player->extremeGear != Airship) return;

    AirshipInfo *AirshipInfo = &PlayerAirshipInfo[player->index];

    if (AirshipInfo->airdashCooldown != 0) {
        AirshipInfo->airdashCooldown -= 1;
    }

    if (player->input->holdFaceButtons & XButton && AirshipInfo->airdashCooldown == 0) {
        if (player->state == Fall || player->state == Jump || player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp) {
            if (AirshipInfo->airdashCharge < 120) {
                AirshipInfo->airdashCharge += 1;
            }
        }
        else {
            AirshipInfo->airdashCharge = 0;
        }
    }else{
        if (AirshipInfo->airdashCharge == 0) return;
        if (player->state == Fall || player->state == Jump || player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp) {
            if (AirshipInfo->airdashCharge < 45) {
                if (AirshipInfo->airdashCooldown == 0 && player->currentAir > player->gearStats[player->level].boostCost * 2) {
                    if (AirshipInfo->airdashCharge >= 90) {
					    player->speed += pSpeed(350);
					}
                    else {
                        player->speed += pSpeed(225);
                    }
                    if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3B)); //Dash panel SFX
                    player->currentAir -= player->gearStats[player->level].boostCost * 2;
                    AirshipInfo->airdashCooldown = 180;
                }
                AirshipInfo->airdashCharge = 0;
            }
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