#include "test_gunbike.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

u32 bike_soundID = 0x0;
u8 test_turnspeed = 0.5f;

void SYB_Player_GunBike(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBGunBikeEXLoad) return;
	if (player->extremeGear != AirTank) return;
	
	player->rings = bike_soundID;

	if (player->input->toggleFaceButtons & DPadUp) {
		bike_soundID += 1;
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, bike_soundID));
		player->unkBAC ^= 0x0100; //Handedness Swap!
    }

	if (player->input->holdFaceButtons & XButton) {
		//if (player->state >= FrontflipRamp && player->state <= TurbulenceTrick) {
		//	player->previousState = player->state;
		//	player->state = Jump;
		//}
		if (player->state == Fall || player->state == Jump) {
			player->genericBool = true;
			player->verticalSpeed = 0;
			player->rings = player->gravity;
			if (player->input->holdFaceButtons & LStickLeft) {
				player->horizontalRotation += test_turnspeed;
			}
			if (player->input->holdFaceButtons & LStickRight) {
				player->horizontalRotation -= test_turnspeed;
			}
			if (player->input->holdFaceButtons & LStickUp) {
				player->verticalRotation += test_turnspeed;
			}
			if (player->input->holdFaceButtons & LStickDown) {
				player->verticalRotation -= test_turnspeed;
			}
		}
	}
	else {
		if (player->state == Fall || player->state == Jump) {
			if (player->genericBool == true) {
				player->state = Fall;
				player->speed += pSpeed(200);
			}
		}
		player->genericBool = false;
	}
}