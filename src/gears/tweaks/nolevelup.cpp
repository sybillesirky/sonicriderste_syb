#include "cosmetics/player/exloads.hpp"

ASMUsed u32 Player_NonLevelupGears(Player *player, u32 specialFlags) {
    //u8 controllerPort = player->input->port;
    const EnabledEXLoads exLoads = FetchEnabledEXLoadIDs(*player);
    if (isSuperCharacter(*player, Tails) || // super tails
		isSuperCharacter(*player, MetalSonic) || // super metal
        player->extremeGear == ExtremeGear::CoverF ||
        player->extremeGear == ExtremeGear::CoverS ||
        player->extremeGear == ExtremeGear::CoverP ||
        player->extremeGear == ExtremeGear::GunGear ||
        exLoads.gearExLoadID == E99EXLoad ||
        exLoads.gearExLoadID == HangOnATEXLoad ||
        player->extremeGear == ExtremeGear::ShootingStar ||
        player->extremeGear == ExtremeGear::WindStar ||
        player->extremeGear == ExtremeGear::RoadStar ||
        player->extremeGear == ExtremeGear::Wanted ||
        player->extremeGear == ExtremeGear::ReserveTank ||
        player->extremeGear == ExtremeGear::Archangel
        //||
        // exLoads.gearExLoadID == TheBeastEXLoad
        ) {
		specialFlags |= ringGear;
	}
    return specialFlags;
}