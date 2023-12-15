#include "airship.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

std::array<AirshipInfo, MaxPlayerCount> PlayerAirshipInfo;

void Player_Airship(Player *player) {

    AirshipInfo *AirshipInfo = &PlayerAirshipInfo[player->index];

    if (AirshipInfo->airdashCooldown != 0) {
        AirshipInfo->airdashCooldown -= 1;
    }

    if (player->input->toggleFaceButtons & XButton) {
        if (player->state == Fall || player->state == Jump || 
            player->state == FrontflipRamp || player->state == BackflipRamp || 
            player->state == ManualRamp || player->state == HalfPipeTrick) {
            if (AirshipInfo->airdashCooldown == 0 && player->currentAir > player->gearStats[player->level].boostCost * 2) {
                player->speed += pSpeed(150);
                player->verticalSpeed -= 0.2;
                // player->verticalSpeed += 0.5; // Air Dash height bonus. Legacy!
                player->currentAir -= player->gearStats[player->level].boostCost * 2;
                AirshipInfo->airdashCooldown = 180;
                if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3B)); //Dash panel SFX
            }
        }
    }

}