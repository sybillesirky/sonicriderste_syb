#include "airship.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

u8 ArSBoostCooldown = 0;
u8 ArSLevelTracker = 0;

constexpr GearLevelStats Level3 = {
		200000, // max air
		32, // air drain
		0x0000014D, // drift cost
		0x61A8, // boost cost
		0x9C40, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(250) // boost speed
};

constexpr GearLevelStats Level2 = {
		150000, // max air
		16, // air drain
		0x000000FA, // drift cost
		0x7530, // boost cost
		0x9C40, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(230) // boost speed
};

constexpr GearLevelStats Level1 = {
		100000, // max air
		16, // air drain
		0x000000A6, // drift cost
		0x9C40, // boost cost
		0x9C40, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(210) // boost speed
};

void Player_Airship_LevelUpdater(Player *player, const GearLevelStats *stats) {
    player->gearStats[player->level].maxAir = stats->maxAir;
    player->gearStats[player->level].airDrain = stats->passiveAirDrain;
    player->gearStats[player->level].driftCost = stats->driftingAirCost;
    player->gearStats[player->level].boostCost = stats->boostCost;
    player->gearStats[player->level].tornadoCost = stats->tornadoCost;
    player->gearStats[player->level].boostSpeed = stats->boostSpeed;
}

void Player_Airship_LevelTracking(Player *player) {
    if (player->level != ArSLevelTracker) {
        switch (player->level) {
            case 2:
                Player_Airship_LevelUpdater(player, &Level3);
                break;
            case 1:
                Player_Airship_LevelUpdater(player, &Level2);
                break;
            default:
                Player_Airship_LevelUpdater(player, &Level1);
        }
        ArSLevelTracker = player->level;
    }
}

void Player_Airship(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBAirshipEXLoad) return;
	if (player->extremeGear != LightBoard) return;
	player->specialFlags = (noSpecialFlags);

    if (ArSBoostCooldown != 0) {
        ArSBoostCooldown -= 1;
    }

    Player_Airship_LevelTracking(player);

    if (player->input->toggleFaceButtons & XButton) {
        if (player->state == Fall || player->state == Jump || player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp) {
            if (ArSBoostCooldown == 0 && player->currentAir > 40000) {
                player->speed += pSpeed(200);
                player->verticalSpeed += 0.1;
                // player->verticalSpeed += 0.5; // Air Dash height bonus. Legacy.
                player->currentAir -= 30000;
                ArSBoostCooldown = 120;
                PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3B)); //Dash panel SFX
            }
        }
    }

    if (player->state == StartLine) { // Initialising behaviours.
        Player_Airship_LevelUpdater(player, &Level1);
	}

}