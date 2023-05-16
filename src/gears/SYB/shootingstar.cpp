#include "shootingstar.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

// IDEA BOARD:
// Does not level up through Rings.
// Has SLIGHTLY better turning, like ZG.
// Faster trick speed, but not as fast as Legend.
// Tracks a global "amount of tricks done" parameter that only increases on successful tricks.
// After certain amounts of tricks done, you level up, up to Level 4.
// Tricks below X carry a Ring penalty. If you can't pay the penalty, you forcefully level down if you have a level.
// SLIGHTLY above average stats at level 1 & 2. Becomes Default at level 3 and only +5 boost speed at level 4. Level 4 is more of a buffer so you can do more S Ranks.

u8 ShS_tricks_accum = 0; // SYB: This keeps track of the total amount of tricks done, for level up.
bool flag_beenTricking = false;

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

constexpr GearLevelStats Level4 = {
		300000, // max air
		300, // air drain
		600, // drift cost
		0x9C40, // boost cost
		0x4E20, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(255) // boost speed
};

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

void ShootingStarLevelHandler(Player *player) {
	// Level tracker
	u8 ShootingStar_level_tracked = player->level;
	if (player->level4 == true) {
		ShootingStar_level_tracked = 4;
	}

	// Set the actual Levels
	if (ShS_tricks_accum >= 30) { // Level 4
		player->level = 2;
		player->level4 = true;
	} 
	else if (ShS_tricks_accum >= 20) { // Level 3
		player->level = 2;
		player->level4 = false;
	} else if (ShS_tricks_accum >= 10) { // Level 2
		player->level = 1;
		player->level4 = false;
	} else { // Level 1
		player->level = 0;
		player->level4 = false;
	}

	// If the levels don't match, reset air, play particles and change stats.
	if (player->level != ShootingStar_level_tracked) {
		if (ShootingStar_level_tracked == 4 && player->level4 == true){return;}
		if (ShootingStar_level_tracked > player->level) {
			Player_CreateShootingStarParticles(player);
			player->currentAir = player->gearStats[player->level].maxAir;
		} else if (ShootingStar_level_tracked < player->level) {
			Player_CreateShootingStarParticles(player);
			player->currentAir = player->gearStats[player->level].maxAir;
		}
		switch (player->level) {
			case 2:
				Player_ShootingStar_UpdateStats(player, &Level3);
				break;
			case 1:
				Player_ShootingStar_UpdateStats(player, &Level2);
				break;
			default:
				Player_ShootingStar_UpdateStats(player, &Level1);
		}
		if (player->level4 == true) {
			player->gearStats[player->level].boostSpeed = pSpeed(255);
			Player_CreateShootingStarParticles(player);
		}
	}
}

void Player_ShootingStar(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBShootingStarEXLoad) return;
	if (player->extremeGear != DefaultGear) return; // SYB: Was going to be Legend but considering its ASM quirks we have to live with Default.
	player->specialFlags |= (noSpeedLossChargingJump);

	ShootingStarTrickMultHandler(player);

	if (flag_beenTricking == true) {
		Player_CreateShootingStarParticles(player);
	}

	if (ShS_tricks_accum > 40) { // No buffer beyond Level 4 allowed.
		ShS_tricks_accum = 40;
	}

	if (player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp || player->state == HalfPipeTrick) {
		flag_beenTricking = true; // This makes sure the Rank check only fires once.
	}

	if (player->previousState == FrontflipRamp || player->previousState == BackflipRamp || player->previousState == ManualRamp || player->previousState == HalfPipeTrick) {
		if (player->state == Cruise || player->state == Fly || player->state == RailGrind) {
			if (player->trickRank != XRank && flag_beenTricking == true) {
				if (player->level > 0) {
					if (player->rings >= 20) {
						player->rings -= 20;
					} else {
						ShS_tricks_accum -= 10;
					}
				}

			}
			ShS_tricks_accum += player->trickCount;
			player->trickCount = 0;
		}
		flag_beenTricking = false;
	}

	ShootingStarLevelHandler(player);

	if (player->state == StartLine) {
		ShS_tricks_accum = 0;
        Player_ShootingStar_UpdateStats(player, &Level1);
		player->level = 0;
        player->currentAir = player->gearStats[player->level].maxAir;
	}
}