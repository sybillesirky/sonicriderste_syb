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
u8 ShS_trickAmount_Lvl2 = 7;
u8 ShS_trickAmount_lvl3 = 14;
u8 ShS_trickAmount_lvl4 = 21;

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

void ShootingStarLevelHandler(Player *player) {
	if (ShS_tricks_accum < 10) {
		player->level = 0;
		player->level4 = false;
	} 
	else if (ShS_tricks_accum < 20) {
		player->level = 1;
		player->level4 = false;
	} else if (ShS_tricks_accum < 30) {
		player->level = 2;
		player->level4 = false;
	} else if (ShS_tricks_accum < 40) {
		player->level = 2;
		player->level4 = true;
	}
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
    player->trickSpeed = player->trickSpeed * 1; // SYB: This is important! Test this hard!
	player->rings = ShS_tricks_accum; // Test.

	ShootingStarLevelHandler(player);

	if (player->trickFail == 10) { // SYB: Tracking C Ranks without giving up our "nothing value"
		player->level += 1;
		player->trickFail = 0;
	}

	if (player->trickRank != CRank) { // C Rank is our "nothing value".
		ShS_tricks_accum += player->trickCount;
		player->trickRank = CRank;
	}

	if (player->state == StartLine) {
        Player_ShootingStar_UpdateStats(player, &Level1);
        player->currentAir = player->gearStats[player->level].maxAir;
	}
}