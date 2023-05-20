#include "reservetank.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

u8 Player_ReserveTank_TankAmount = 2; // Doesn't track Level 4 because that one is not problematic.

constexpr GearLevelStats Level3 = {
		300000, // max air
		120, // air drain
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
    player->gearStats[player->level].airDrain = stats->passiveAirDrain;
    player->gearStats[player->level].driftCost = stats->driftingAirCost;
    player->gearStats[player->level].boostCost = stats->boostCost;
    player->gearStats[player->level].tornadoCost = stats->tornadoCost;
    player->gearStats[player->level].boostSpeed = stats->boostSpeed;
    player->shortcutAirGainMultiplier = 0;
    player->unk9C8 = 0x3ecccccd; // This is the trick air gain mult. Currently 0.4.
    player->unk9D0 = 0x3f800000; // This is the QTE air gain mult. Currently 1.
}

void Player_ReserveTank_SetBaseStats(Player *player) {
    if (player->specialFlags != (noPits)) {
        player->specialFlags = (noPits);
        Player_ReserveTank_UpdateStats(player, &Level3);
		player->level = 2;
        player->level4 = true;
        player->currentAir = player->gearStats[player->level].maxAir;
        Player_ReserveTank_TankAmount = 2;
    }
}

void Player_ReserveTank(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBReserveTankEXLoad) return;
	if (player->extremeGear != AirTank) return;

	if (player->state == StartLine) { // Initialising behaviour.
        Player_ReserveTank_SetBaseStats(player);
	}

	if (player->input->toggleFaceButtons & DPadUp) {
        if (player->rings < 20) return;
        if (player->level4 == true) {
            PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x39)); // Ring loss SFX
		    player->level4 = false;
            player->rings -= 20;
            Player_ReserveTank_UpdateStats(player, &Level3);
            Player_CreateReserveTankParticles(player);
		    player->currentAir = player->gearStats[player->level].maxAir;
        }else if (player->level <= 0) {
			return;
            }else {
                PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x39)); //Ring loss SFX
                player->level -= 1;
                player->rings -= 20;
                Player_ReserveTank_TankAmount -= 1;
                Player_ReserveTank_UpdateStats(player, &Level3);
                Player_CreateReserveTankParticles(player);
		        player->currentAir = player->gearStats[player->level].maxAir;
                }
    }

    if (player->previousState == Death) {
        player ->level = Player_ReserveTank_TankAmount;
    }

}