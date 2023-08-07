#include "test_gunbike.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

void SYB_Player_GunBike(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBGunBikeEXLoad) return;
	if (player->extremeGear != AirTank) return;
	
	player->rings = 100;
}