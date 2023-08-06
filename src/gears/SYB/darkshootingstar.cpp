#include "darkshootingstar.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

// u8 ShS_tricks_accum = 0; // This keeps track of the total amount of tricks done, for level up. THIS IS NOW u8 SYBArSShSCounter.
// bool flag_beenTricking = false; // Roundabout way to determine a trick has happened. THIS IS NOW BOOL SYBBeenTricking.
// u8 ShS_stored_level = 0; THIS IS NOW SYBShSRsTLevelTracker.

DarkShootingStarInfo PlayerDarkShootingStarInfo[8];

void Player_CreateDarkShootingStarParticles(Player *player) {
    auto *particles = reinterpret_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, 0xB3B0, 2)->object);
    particles->unk72 = player->index;
    particles->unk0 = 0.0f;
    particles->unk8 = 0.0f;
    particles->unk10 = 0.0f;
    particles->unk18 = 0.0f;
    particles->unk4 = 0.3f;
    particles->unk14 = 0.8f;
    particles->unk68 = gpsTexList_Particle;
    particles->unk60 = &lbl_001D8F58;
    particles->unk73 = 0xFF;
    particles->unk6C = 0;
    particles->unk74 = 0;
    particles->unk48 = &player->x;
}

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

void Player_DarkShootingStar_LevelUpdater(Player *player, const GearLevelStats *stats, int inputLevel) {
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

void Player_DarkShootingStar_SetStats(Player *player) {
    if (player->gearStats[0].boostSpeed != pSpeed(210)) {
        player->specialFlags &= ~noSpeedLossChargingJump;
		player->specialFlags &= ~iceImmunity;
        Player_DarkShootingStar_LevelUpdater(player, &Level1, 0);
        Player_DarkShootingStar_LevelUpdater(player, &Level2, 1);
        Player_DarkShootingStar_LevelUpdater(player, &Level3, 2);
    }
}

void Player_DarkShootingStar(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

    DarkShootingStarInfo *DShSInfo = &PlayerDarkShootingStarInfo[player->index];

    player->rings = DShSInfo->trickAccumulator;

	if (exLoads.gearExLoadID != SYBDarkShootingStarEXLoad) return;
	if (player->extremeGear != DefaultGear) return; // SYB: Was going to be Legend but considering its ASM quirks we have to live with Default.

	// Ensure player never gets a buffer of tricks beyond Level 3.
	if (DShSInfo->trickAccumulator > 20) {
		DShSInfo->trickAccumulator = 20;
	}

	// Basically define "player is in trick state".
	if (player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp || player->state == HalfPipeTrick) {
		DShSInfo->beenTricking = true; // This makes sure trick landing behaviours only fire once.
	}

	// What happens once player has left trick state.
	if (player->previousState == FrontflipRamp || player->previousState == BackflipRamp || player->previousState == ManualRamp || player->previousState == HalfPipeTrick) {
		if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {
			DShSInfo->trickAccumulator += player->trickCount;

			// If trick rank is lower than X, induce penalties if level 2 or higher.
			if (player->trickCount < 4 && 
				DShSInfo->beenTricking == true &&
				player->state == Cruise) { // SYB: No more P2W if Fly/Grind.
				if (player->level > 0) {
					// Activate mode
                    DShSInfo->P2WActive = true;
                    DShSInfo->P2WFrames += 300;

                    // Set new boost speeds
                    player->gearStats[0].boostSpeed = pSpeed(230);
			        player->gearStats[1].boostSpeed = pSpeed(250);
			        player->gearStats[2].boostSpeed = pSpeed(270);

					if (player->rings >= 20) {
						player->rings -= 20;
						if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x39)); // Ring loss SFX
					} else {
						DShSInfo->trickAccumulator -= 10;
					}
				}

			}
			player->trickCount = 0;
		}

		if (DShSInfo->beenTricking == true) { // Prevents constant updating.
			if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {
				DShSInfo->levelHolder = player->level;

				// Update the level and stats now that we have the new amount of tricks.
				if (DShSInfo->trickAccumulator >= 20) { // Level 3
					player->level = 2;
				} else if (DShSInfo->trickAccumulator >= 10) { // Level 2
					player->level = 1;
				} else { // Level 1
					player->level = 0;
				}	

				//If the level was changed, refill air gauge and play VFX/SFX.
				if (player->level != DShSInfo->levelHolder) {
					player->currentAir = player->gearStats[player->level].maxAir;
					if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0xD)); // Levelling SFX
					Player_CreateDarkShootingStarParticles(player);
				}
			}
		}

		// Specify that player is no longer in trick state.
		DShSInfo->beenTricking = false;
	}

	if (player->state == StartLine) { // Initialising behaviours.
		DShSInfo->trickAccumulator = 0;
        Player_DarkShootingStar_SetStats(player);
		DShSInfo->beenTricking = false;
	}

	if (player->previousState == Death) { // Apply Level again on death.
		if (DShSInfo->trickAccumulator >= 20) { // Level 3
			player->level = 2;
		} else if (DShSInfo->trickAccumulator >= 10) { // Level 2
			player->level = 1;
		} else { // Level 1
			player->level = 0;
		}
		player->previousState = Cruise;
	}

    // P2W Mode deactivation.
    if (DShSInfo->P2WActive == true && DShSInfo->P2WFrames == 0) {
        DShSInfo->P2WActive = false;
        Player_DarkShootingStar_SetStats(player);
    }

    // P2W Mode Counter decrease.
	if (DShSInfo->P2WActive == true) {
		DShSInfo->P2WFrames -= 1;
	}
}