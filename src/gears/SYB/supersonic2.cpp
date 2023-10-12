#include "supersonic2.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

SuperSonic2Info PlayerSuperSonic2Info[8];

constexpr GearLevelStats Level3 = {
	200000, // max air
	64, // air drain
	100, // drift cost
	0x9C40, // boost cost
	0x9C40, // tornado cost
	pSpeed(100), // drift dash speed, unused
	pSpeed(250) // boost speed
};

constexpr GearLevelStats Level2 = {
	150000, // max air
	32, // air drain
	100, // drift cost
	0x9C40, // boost cost
	0x9C40, // tornado cost
	pSpeed(100), // drift dash speed, unused
	pSpeed(230) // boost speed
};

constexpr GearLevelStats Level1 = {
	100000, // max air
	16, // air drain
	100, // drift cost
	0x7530, // boost cost
	0x7530, // tornado cost
	pSpeed(100), // drift dash speed, unused
	pSpeed(210) // boost speed
};

void Player_SuperSonic2_LevelUpdater(Player *player, const GearLevelStats *stats, int inputLevel) {
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

void Player_SuperSonic2_SetStats(Player *player) {
    if (player->gearStats[0].boostSpeed != pSpeed(210)) {
        player->specialFlags &= ~noSpeedLossChargingJump;
		player->specialFlags &= ~iceImmunity;
        Player_SuperSonic2_LevelUpdater(player, &Level1, 0);
        Player_SuperSonic2_LevelUpdater(player, &Level2, 1);
        Player_SuperSonic2_LevelUpdater(player, &Level3, 2);
    }
}

void Player_SuperSonic2(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SuperSonic2EXLoad) return;
	if (player->extremeGear != ChaosEmerald) return;

    SuperSonic2Info *SS2Info = &PlayerSuperSonic2Info[player->index];

	// Ensure player never gets a buffer of tricks beyond Level 3.
	if (SS2Info->trickAccumulator > 20) {
		SS2Info->trickAccumulator = 20;
	}

    // Blast Gauge display.
    player->shortcutAirGainMultiplier = static_cast<s32>(SS2Info->trickAccumulator) * 10000;

	// Basically define "player is in trick state".
	if (player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp || player->state == HalfPipeTrick) {
		SS2Info->beenTricking = true; // This makes sure trick landing behaviours only fire once.
	}

	// What happens once player has left trick state.
	if (player->previousState == FrontflipRamp || player->previousState == BackflipRamp || player->previousState == ManualRamp || player->previousState == HalfPipeTrick) {
		if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {
			SS2Info->trickAccumulator += player->trickCount;
			player->trickCount = 0;
		}

		if (SS2Info->beenTricking == true) { // Prevents constant updating.
			if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {
				SS2Info->levelHolder = player->level;

				// Update the level and stats now that we have the new amount of tricks.
				if (SS2Info->trickAccumulator >= 20) { // Level 3
					player->level = 2;
				} else if (SS2Info->trickAccumulator >= 10) { // Level 2
					player->level = 1;
				} else { // Level 1
					player->level = 0;
				}	

				//If the level was changed, refill air gauge and play VFX/SFX.
				if (player->level != SS2Info->levelHolder) {
					if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0xD)); // Levelling SFX
					// Player_CreateDarkShootingStarParticles(player);
				}
			}
		}

		// Specify that player is no longer in trick state.
		SS2Info->beenTricking = false;
	}

	if (player->state == StartLine) { // Initialising behaviours.
		SS2Info->trickAccumulator = 0;
        Player_SuperSonic2_SetStats(player);
		SS2Info->beenTricking = false;
	}

	if (player->previousState == Death) { // Apply Level again on death.
		if (SS2Info->trickAccumulator >= 20) { // Level 3
			player->level = 2;
		} else if (SS2Info->trickAccumulator >= 10) { // Level 2
			player->level = 1;
		} else { // Level 1
			player->level = 0;
		}
		player->previousState = Cruise;
	}
}