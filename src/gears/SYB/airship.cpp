#include "airship.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

AirshipInfo PlayerAirshipInfo[8];

constexpr GearLevelStats Level3 = {
		200000, // max air
		32, // air drain
		0x0000014D, // drift cost
		0x4E20, // boost cost
		0x9C40, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(245) // boost speed
};

constexpr GearLevelStats Level2 = {
		150000, // max air
		16, // air drain
		0x000000FA, // drift cost
		0x4E20, // boost cost
		0x7530, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(225) // boost speed
};

constexpr GearLevelStats Level1 = {
		100000, // max air
		16, // air drain
		0x000000A6, // drift cost
		0x3A98, // boost cost
		0x61A8, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(195) // boost speed
};

void Player_Airship_LevelUpdater(Player *player, const GearLevelStats *stats, int inputLevel) {
    player->gearStats[inputLevel].maxAir = stats->maxAir;
    player->gearStats[inputLevel].airDrain = stats->passiveAirDrain;
    player->gearStats[inputLevel].driftCost = stats->driftingAirCost;
    player->gearStats[inputLevel].boostCost = stats->boostCost;
    player->gearStats[inputLevel].tornadoCost = stats->tornadoCost;
    player->gearStats[inputLevel].boostSpeed = stats->boostSpeed;
    if (player->characterArchetype == BoostArchetype) {
		 player->gearStats[inputLevel].boostSpeed += BoostArchetypeBoostSpeeds[inputLevel];
	}
}

void Player_Airship_SetStats(Player *player) {
    if (player->specialFlags != (noSpecialFlags)) {
        player->specialFlags = (noSpecialFlags);
        Player_Airship_LevelUpdater(player, &Level1, 0);
        Player_Airship_LevelUpdater(player, &Level2, 1);
        Player_Airship_LevelUpdater(player, &Level3, 2);
    }
}

void Player_Airship(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	//if (exLoads.gearExLoadID != SYBAirshipEXLoad) return;
	//if (player->extremeGear != LightBoard) return;
    if (player->extremeGear != TestGear) return;

    AirshipInfo *AirshipInfo = &PlayerAirshipInfo[player->index];

    if (AirshipInfo->airdashCooldown != 0) {
        AirshipInfo->airdashCooldown -= 1;
    }

    if (player->input->toggleFaceButtons & XButton) {
        if (player->state == Fall || player->state == Jump || player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp) {
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

    //if (player->state == StartLine) { // Initialising behaviours.
    //    Player_Airship_SetStats(player);
	//}

}