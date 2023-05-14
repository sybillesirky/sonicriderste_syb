#include "test_gunbike.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"

void SYB_test_unleashedBoost(Player *player) {
	f32 boostSpeed;

	boostSpeed = pSpeed(300);
    player->fastest_superCruise = TRUE;
	if (player->state == Jump) {
		player->currentAir -= 2000;
		player->speed = boostSpeed;
		player->specialFlags |= berserkerEffect;
	}else if (player->state == Cruise) {
		player->currentAir -= 700;
		player->speed = boostSpeed;
		player->specialFlags |= berserkerEffect;
	}
}

void SYB_Player_GunBike(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	u32 playerTrackedRingCount = 0;

	if (exLoads.gearExLoadID != SYBGunBikeEXLoad) return;
	if (player->extremeGear != AirTank) return;
	player->specialFlags |= (noBoosting);

	if (player->state == StartLine) {
		player->level = 2;
	}

//	if (player->input->toggleFaceButtons & XButton) {
//        if (player->level <= 0) {
//			return;
//		}
//		player->level -= 1;
//		player->currentAir = player->gearStats[player->level].maxAir;
//    }

	if (player->input->holdFaceButtons & BButton) {
		SYB_test_unleashedBoost(player);
    }else {
		player->fastest_superCruise = FALSE;
		player->specialFlags &= ~berserkerEffect;
	}

	//while (playerTrackedRingCount != player->rings) {
	//	player->currentAir += 1000;
	//	playerTrackedRingCount -= 1;
	//}
}