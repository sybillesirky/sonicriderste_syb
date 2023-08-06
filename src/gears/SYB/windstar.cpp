#include "windstar.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

WindStarInfo PlayerWindStarInfo[8];

void Player_CreateWindStarParticles(Player *player) {
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

constexpr GearLevelStats Level1 = {
	100000, // max air
	16, // air drain
	0x000000A6, // drift cost
	0x61A8, // boost cost
	0x61A8, // tornado cost
	pSpeed(500), // drift dash speed, unused
	pSpeed(200) // boost speed
};

constexpr GearLevelStats Level2 = {
	150000, // max air
	16, // air drain
	0x000000FA, // drift cost
	0x7530, // boost cost
	0x61A8, // tornado cost
	pSpeed(500), // drift dash speed, unused
	pSpeed(225) // boost speed
};

constexpr GearLevelStats Level3 = {
	200000, // max air
	16, // air drain
	0x0000014D, // drift cost
	0x9C40, // boost cost
	0x61A8, // tornado cost
	pSpeed(500), // drift dash speed, unused
	pSpeed(240) // boost speed
};

void Player_WindStar_LevelUpdater(Player *player, const GearLevelStats *stats, int inputLevel) {
    player->gearStats[inputLevel].maxAir = stats->maxAir;
    player->gearStats[inputLevel].airDrain = stats->passiveAirDrain;
    player->gearStats[inputLevel].driftCost = stats->driftingAirCost;
    player->gearStats[inputLevel].boostCost = stats->boostCost;
    player->gearStats[inputLevel].tornadoCost = stats->tornadoCost;
    //player->gearStats[inputLevel].driftDashSpeed = stats->driftDashSpeed;
    player->gearStats[inputLevel].boostSpeed = stats->boostSpeed;
	if (player->characterArchetype == BoostArchetype) {
		player->gearStats[inputLevel].boostSpeed += BoostArchetypeBoostSpeeds[inputLevel];
	}
}

void Player_WindStar_SetStats(Player *player) {
	player->specialFlags = (tornadoBoost);
    Player_WindStar_LevelUpdater(player, &Level1, 0);
    Player_WindStar_LevelUpdater(player, &Level2, 1);
    Player_WindStar_LevelUpdater(player, &Level3, 2);
}

void Player_WindStar(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBWindStarEXLoad) return;
	if (player->extremeGear != AutoSlider) return;

	WindStarInfo *WdSInfo = &PlayerWindStarInfo[player->index];

	// Ensure player never gets a buffer of tricks beyond Level 3.
	if (WdSInfo->trickAccumulator > 24) {
		WdSInfo->trickAccumulator = 24;
	}

	// Basically define "player is in trick state".
	if (player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp || player->state == HalfPipeTrick) {
		WdSInfo->beenTricking = true; // This makes sure trick landing behaviours only fire once.
	}

	// What happens once player has left trick state.
	if (player->previousState == FrontflipRamp || player->previousState == BackflipRamp || player->previousState == ManualRamp || player->previousState == HalfPipeTrick) {
		if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {
			WdSInfo->trickAccumulator += player->trickCount;
			player->trickCount = 0;
		}

		if (WdSInfo->beenTricking == true) { // Prevents constant updating.
			if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {
				WdSInfo->levelHolder = player->level;

				// Update the level and stats now that we have the new amount of tricks.
				if (WdSInfo->trickAccumulator >= 24) { // Level 3
					player->level = 2;
				} else if (WdSInfo->trickAccumulator >= 12) { // Level 2
					player->level = 1;
				} else { // Level 1
					player->level = 0;
				}	

				//If the level was changed, refill air gauge and play VFX/SFX.
				if (player->level != WdSInfo->levelHolder) {
					player->currentAir = player->gearStats[player->level].maxAir;
					if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0xD)); // Levelling SFX
					Player_CreateWindStarParticles(player);
				}
			}
		}

		// Specify that player is no longer in trick state.
		WdSInfo->beenTricking = false;
	}

	// Initialising behaviours.
	if (player->state == StartLine) {
		WdSInfo->trickAccumulator = 0;
		if (player->gearStats[1].boostSpeed != pSpeed(225)) {
			Player_WindStar_SetStats(player);
    	}
		WdSInfo->beenTricking = false;
	}

	// Tailwind Mode Activation.
	if (player->unk1040 == 1 && WdSInfo->tailwindActive == false) { // If we did a tornado while not transformed.
		if (player->rings >= 1) {
			WdSInfo->tailwindFrames = 20;
			WdSInfo->tailwindActive = true; // Activate mode.
			player->speed += pSpeed(50);
			PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3D)); // Speed Shoes SFX

			// Update Boost Speeds
			player->gearStats[0].boostSpeed += pSpeed(20);
			player->gearStats[1].boostSpeed += pSpeed(20);
			player->gearStats[2].boostSpeed += pSpeed(20);
		}
    }

	// Tailwind Mode Deactivation.
	if (WdSInfo->tailwindActive == true && player->rings == 0) {
		Player_WindStar_SetStats(player);
		WdSInfo->tailwindActive = false;
	}

	// Tailwind: Deactivate when QTE/Death.
	if (player->state == QTE || player->state == QTE2 || player->state == Death) {
		if (WdSInfo->tailwindActive == true) {
			Player_WindStar_SetStats(player);
			WdSInfo->tailwindActive = false;
		}
	}

	// Tailwind Mode Buffs.
	if (WdSInfo->tailwindActive == true && player->state == Cruise) {
		player->currentAir += 200;
		if (WdSInfo->tailwindFrames <= 0) {
			player->rings -= 1;
			WdSInfo->tailwindFrames = 21;
		}
		WdSInfo->tailwindFrames -= 1;
	}

	if (player->previousState == Death) { // Apply Level again on death.
		if (WdSInfo->trickAccumulator >= 24) { // Level 3
			player->level = 2;
		} else if (WdSInfo->trickAccumulator >= 12) { // Level 2
			player->level = 1;
		} else { // Level 1
			player->level = 0;
		}
		player->previousState = Cruise;
	}
}