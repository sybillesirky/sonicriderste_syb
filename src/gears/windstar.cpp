#include "windstar.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

std::array<WindStarInfo, MaxPlayerCount> PlayerWindStarInfo;

void Player_WindStar(Player *player) {
    
    WindStarInfo *WdSInfo = &PlayerWindStarInfo[player->index];

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
		player->gearStats[0].boostSpeed -= pSpeed(20);
		player->gearStats[1].boostSpeed -= pSpeed(20);
		player->gearStats[2].boostSpeed -= pSpeed(20);
		WdSInfo->tailwindActive = false;
	}

	// Tailwind: Deactivate when QTE/Death.
	if (player->state == QTE || player->state == QTE2 || player->state == Death) {
		if (WdSInfo->tailwindActive == true) {
			player->gearStats[0].boostSpeed -= pSpeed(20);
		    player->gearStats[1].boostSpeed -= pSpeed(20);
		    player->gearStats[2].boostSpeed -= pSpeed(20);
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
}