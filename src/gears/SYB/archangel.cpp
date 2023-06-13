#include "archangel.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

void Player_ArchAngel(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBArchAngelEXLoad) return;
	if (player->extremeGear != DefaultGear) return;

	if (player->input->toggleFaceButtons & DPadUp) {
		player->unkBAC ^= 0x0100; //Handedness Swap!
    }
}