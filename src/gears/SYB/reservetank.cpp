#include "reservetank.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

constexpr GearLevelStats Level3 = {
		300000, // max air
		300, // air drain
		600, // drift cost
		0x9C40, // boost cost
		0x4E20, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(250) // boost speed
};

void Player_CreateReserveTankParticles(Player *player) {
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

void Player_ReserveTank_UpdateStats(Player *player, const GearLevelStats *stats) {
    player->gearStats[player->level].maxAir = stats->maxAir;
    player->gearStats[player->level].driftCost = stats->driftingAirCost;
    player->gearStats[player->level].boostCost = stats->boostCost;
    player->gearStats[player->level].tornadoCost = stats->tornadoCost;
    player->gearStats[player->level].boostSpeed = stats->boostSpeed;
    player->shortcutAirGainMultiplier = 0.5;
}

void Player_ReserveTank(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBReserveTankEXLoad) return;
	if (player->extremeGear != AirTank) return;
	player->specialFlags |= (noPits);

	if (player->state == StartLine) {
        Player_ReserveTank_UpdateStats(player, &Level3);
		player->level = 2;
        player->level4 = true;
        player->currentAir = player->gearStats[player->level].maxAir;
	}

	if (player->input->toggleFaceButtons & DPadUp) {
        if (player->level4 == true) {
            PlayAudioFromDAT(Sound::SFX::TornadoHit);
		    player->level4 = false;
            Player_ReserveTank_UpdateStats(player, &Level3);
            Player_CreateReserveTankParticles(player);
		    player->currentAir = player->gearStats[player->level].maxAir;
        }else if (player->level <= 0) {
			return;
            }else {
                PlayAudioFromDAT(Sound::SFX::TornadoHit);
                player->level -= 1;
                Player_ReserveTank_UpdateStats(player, &Level3);
                Player_CreateReserveTankParticles(player);
		        player->currentAir = player->gearStats[player->level].maxAir;
                }
    }

}