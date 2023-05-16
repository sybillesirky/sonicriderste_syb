#include "shootingstar.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

u8 ShS_tricks_accum = 0; // This keeps track of the total amount of tricks done, for level up.
bool flag_beenTricking = false; // Roundabout way to determine a trick has happened.

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
		300000, // max air
		300, // air drain
		600, // drift cost
		0x9C40, // boost cost
		0x4E20, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(250) // boost speed
};

constexpr GearLevelStats Level2 = {
		200000, // max air
		300, // air drain
		600, // drift cost
		0x9C40, // boost cost
		0x4E20, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(240) // boost speed
};

constexpr GearLevelStats Level1 = {
		100000, // max air
		300, // air drain
		100, // drift cost
		0x9C40, // boost cost
		0x4E20, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(210) // boost speed
};

void ShootingStarTrickMultHandler(Player *player) {
	player->trickSpeed = player->trickSpeed * 1;
}

void Player_ShootingStar_UpdateStats(Player *player, const GearLevelStats *stats) {
    player->gearStats[player->level].maxAir = stats->maxAir;
    player->gearStats[player->level].driftCost = stats->driftingAirCost;
    player->gearStats[player->level].boostCost = stats->boostCost;
    player->gearStats[player->level].tornadoCost = stats->tornadoCost;
    player->gearStats[player->level].boostSpeed = stats->boostSpeed;
}

void Player_ShootingStar(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBShootingStarEXLoad) return;
	if (player->extremeGear != DefaultGear) return; // SYB: Was going to be Legend but considering its ASM quirks we have to live with Default.
	player->specialFlags |= (noSpeedLossChargingJump);

	// Trick speed bonus handler.
	ShootingStarTrickMultHandler(player);

	// Debug visualisation.
	if (flag_beenTricking == true) {
		Player_CreateShootingStarParticles(player);
	}
	player->rings = ShS_tricks_accum; // Test.

	// Ensure player never gets a buffer of tricks beyond Level 4.
	if (ShS_tricks_accum > 30) {
		ShS_tricks_accum = 30;
	}

	// Basically define "player is in trick state".
	if (player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp || player->state == HalfPipeTrick) {
		flag_beenTricking = true; // This makes sure the Rank check only fires once.
	}

	// What happens once player has left trick state.
	if (player->previousState == FrontflipRamp || player->previousState == BackflipRamp || player->previousState == ManualRamp || player->previousState == HalfPipeTrick) {
		if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {

			// Add the trick count, then set it back to zero.
			ShS_tricks_accum += player->trickCount;
			player->trickCount = 0; // Double check this to see if this has influence on Rank.

			// If trick rank is lower than X, then induce penalties if level 2 or higher.
			if (player->trickRank != XRank && flag_beenTricking == true) {
				if (player->level > 0) {
			//		if (player->rings >= 20) {
			//			player->rings -= 20;
			//		} else {
			//			ShS_tricks_accum -= 10;
			//		}
						ShS_tricks_accum -= 10;
				}

			}
		}

		if (flag_beenTricking == true) { // Prevents constant updating.

			u8 ShS_stored_level = player->level;
			if (player->level4 == true) {
				ShS_stored_level = 3;
			}

			// Update the level and stats now that we have the new amount of tricks.
			if (ShS_tricks_accum >= 30) { // Level 4
				player->level = 2;
				player->level4 = true;
				Player_ShootingStar_UpdateStats(player, &Level3);
				player->gearStats[player->level].boostSpeed = pSpeed(255);
			} else if (ShS_tricks_accum >= 20) { // Level 3
				player->level = 2;
				player->level4 = false;
				Player_ShootingStar_UpdateStats(player, &Level3);
			} else if (ShS_tricks_accum >= 10) { // Level 2
				player->level = 1;
				player->level4 = false;
				Player_ShootingStar_UpdateStats(player, &Level2);
			} else { // Level 1
				player->level = 0;
				player->level4 = false;
				Player_ShootingStar_UpdateStats(player, &Level1);
			}

			//If the level was changed, refill air gauge and play particles.
			if (player->level != ShS_stored_level) {
				if (player->level4 == true && ShS_stored_level == 3) {
					player->level4 = true;
				} else {
					player->currentAir = player->gearStats[player->level].maxAir;
					PlayAudioFromDAT(Sound::SFX::TornadoHit);
				}
			}
		}

		// Specify that player is no longer in trick state.
		flag_beenTricking = false;
	}

	if (player->state == StartLine) { // Initialising behaviours.
		ShS_tricks_accum = 0;
        Player_ShootingStar_UpdateStats(player, &Level1);
		player->level = 0;
		player->level4 = false;
        player->currentAir = player->gearStats[player->level].maxAir;
	}
}