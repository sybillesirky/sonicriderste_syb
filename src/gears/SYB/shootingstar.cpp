#include "shootingstar.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

// u8 ShS_tricks_accum = 0; // This keeps track of the total amount of tricks done, for level up. THIS IS NOW u8 SYBArSShSCounter.
// bool flag_beenTricking = false; // Roundabout way to determine a trick has happened. THIS IS NOW BOOL SYBBeenTricking.
// u8 ShS_stored_level = 0; THIS IS NOW SYBShSRsTLevelTracker.

void Player_CreateShootingStarParticles(Player *player) {
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

void Player_ShootingStar_LevelUpdater(Player *player, const GearLevelStats *stats, int inputLevel) {
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

void Player_ShootingStar_SetStats(Player *player) {
    if (player->specialFlags != (noSpeedLossTurning)) {
        player->specialFlags = (noSpeedLossTurning);
        Player_ShootingStar_LevelUpdater(player, &Level1, 0);
        Player_ShootingStar_LevelUpdater(player, &Level2, 1);
        Player_ShootingStar_LevelUpdater(player, &Level3, 2);
    }
}

void Player_ShootingStar(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBShootingStarEXLoad) return;
	if (player->extremeGear != DefaultGear) return; // SYB: Was going to be Legend but considering its ASM quirks we have to live with Default.

	// Ensure player never gets a buffer of tricks beyond Level 3.
	if (player->genericCounter1 > 20) {
		player->genericCounter1 = 20;
	}

	// Basically define "player is in trick state".
	if (player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp || player->state == HalfPipeTrick) {
		player->genericBool = true; // This makes sure trick landing behaviours only fire once.
	}

	// What happens once player has left trick state.
	if (player->previousState == FrontflipRamp || player->previousState == BackflipRamp || player->previousState == ManualRamp || player->previousState == HalfPipeTrick) {
		if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {
			player->genericCounter1 += player->trickCount;

			// If trick rank is lower than X, induce penalties if level 2 or higher.
			if (player->trickCount < 4 && player->genericBool == true) {
				if (player->level > 0) {
					player->speed += pSpeed(20);
					if (player->rings >= 20) {
						player->rings -= 20;
						if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x39)); // Ring loss SFX
					} else {
						player->genericCounter1 -= 10;
					}
				}

			}
			player->trickCount = 0;
		}

		if (player->genericBool == true) { // Prevents constant updating.
			if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {
				player->genericCounter2 = player->level;

				// Update the level and stats now that we have the new amount of tricks.
				if (player->genericCounter1 >= 20) { // Level 3
					player->level = 2;
				} else if (player->genericCounter1 >= 10) { // Level 2
					player->level = 1;
				} else { // Level 1
					player->level = 0;
				}	

				//If the level was changed, refill air gauge and play VFX/SFX.
				if (player->level != player->genericCounter2) {
					player->currentAir = player->gearStats[player->level].maxAir;
					if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0xD)); // Levelling SFX
					Player_CreateShootingStarParticles(player);
				}
			}
		}

		// Specify that player is no longer in trick state.
		player->genericBool = false;
	}

	if (player->state == StartLine) { // Initialising behaviours.
		player->genericCounter1 = 0;
        Player_ShootingStar_SetStats(player);
		player->genericBool = false;
	}
}