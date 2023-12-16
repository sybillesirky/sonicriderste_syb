#include "reservetank.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"
#include "riders/object.hpp"

std::array<ReserveTankInfo, MaxPlayerCount> PlayerReserveTankInfo;

void Player_CreateReserveTankParticles(Player *player) {
    auto *particles = static_cast<ParticleTaskObject1 *>(SetTask(func_Particle_Task, ObjectGroups::Particle, 2)->object);
	particles->unk72 = player->index;
	particles->x = 0.0f;
	particles->z = 0.0f;
	particles->unk10 = 0.0f;
	particles->unk18 = 0.0f;
	particles->y = 0.3f;
	particles->unk14 = 0.8f;
	particles->unk68 = gpsTexList_Particle;
	particles->unk60 = &lbl_001D8F58;
	particles->unk73 = static_cast<u8>(~0);
	particles->unk6C = nullptr;
	particles->unk74 = 0;
	particles->unk48 = &player->x;
}

void Player_ReserveTank(Player *player) {
    ReserveTankInfo *RsTInfo = &PlayerReserveTankInfo[player->index];

    // Initialising behaviours
    if (player->state == StartLine) {
        player->level = 2;
        player->gearSpecificFlags[ReserveTank::Level4] = true;
        RsTInfo->tankAmount = 3;
        RsTInfo->refillActive = false;
    }

    // Fix level upon death
    if (player->previousState == Death) {
        switch (RsTInfo->tankAmount) {
            case 3:
                player->level = 2;
                player->gearSpecificFlags[ReserveTank::Level4] = true;
                break;
            case 2:
                player->level = 2;
                player->gearSpecificFlags[ReserveTank::Level4] = false;
                break;
            case 1:
                player->level = 1;
                player->gearSpecificFlags[ReserveTank::Level4] = false;
                break;
            default:
                player->level = 0;
                player->gearSpecificFlags[ReserveTank::Level4] = false;
                break;
        }
        player->previousState = Cruise;
    }

    // Refill Active behaviour
    if (RsTInfo->refillActive == true) {
        // Turn off refill if at max air
        if (player->currentAir == player->gearStats[player->level].maxAir) {
            RsTInfo->refillActive = false;
            player->gearStats[player->level].boostCost = Gears[ExtremeGear::ReserveTank].levelStats[0].boostCost;
            if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x12)); // Exit Pit SFX
        }
        else {
            player->currentAir += 1500;
        }
    }

    // Refill Tank Button
    if (player->input->toggleFaceButtons.hasAny(DPadUp) && player->rings >= 25 && player->level != 0) {

        // Lower the level
        if (player->gearSpecificFlags[ReserveTank::Level4] == true) {
            player->gearSpecificFlags[ReserveTank::Level4] = false;
        } else {
            player->level -= 1;
        }
        player->rings -= 25;
        RsTInfo->tankAmount -= 1;
        Player_CreateReserveTankParticles(player);

        // Start Refilling state & SFX
        RsTInfo->refillActive = true;
        player->gearStats[player->level].boostCost = 0x0;
        if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x39)); // Ring loss SFX
    }
}