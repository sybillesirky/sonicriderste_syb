#include "test_gunbike.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

u32 bike_soundID = 0x34;

void SYB_Player_GunBike(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBGunBikeEXLoad) return;
	if (player->extremeGear != AirTank) return;
	
	player->rings = bike_soundID;

	if (player->input->toggleFaceButtons & DPadUp) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, bike_soundID));
		//bike_soundID += 1;
    }
}