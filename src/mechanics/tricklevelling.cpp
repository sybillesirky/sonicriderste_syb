#include "tricklevelling.hpp"
#include "lib/sound.hpp"
#include "riders/object.hpp"

std::array<TrickLevelInfo, MaxPlayerCount> PlayerTrickLevelInfo;

void Player_CreateTrickLevelUpParticles(Player *player) {
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

void Player_TrickLevelling(Player *player) {

    TrickLevelInfo *TrLInfo = &PlayerTrickLevelInfo[player->index];

    std::array<u8, 2> Tricks4Level = {0, 0};
    u8 startingLevel = player->level;

    switch(player->extremeGear) {
        using namespace ExtremeGear;
        case WindStar:
            Tricks4Level = {12, 24};
            break;
        case ShootingStar:
        case RoadStar:
            Tricks4Level = {10, 20};
            break;
        default:
            break;
    }

    if (player->state == StartLine) {
        TrLInfo->trickAccumulator = 0;
        TrLInfo->isTricking = false;
    }

    if (player->state == FrontflipRamp || player->state == BackflipRamp || player->state == ManualRamp || player->state == HalfPipeTrick) {
        TrLInfo->isTricking = true; // This makes sure trick landing behaviours only fire once.
    }

    // What happens once the player has left trick state.
    if (player->previousState == FrontflipRamp || player->previousState == BackflipRamp || player->previousState == ManualRamp || player->previousState == HalfPipeTrick) {
        if ((player->state == Cruise || player->state == Fly || player->state == RailGrind) && (TrLInfo->isTricking == true)) { // Prevents constant updating

            // Update the accumulator and check the cap
			TrLInfo->trickAccumulator += player->trickCount;
            if (TrLInfo->trickAccumulator > Tricks4Level[1]) {
                TrLInfo->trickAccumulator = Tricks4Level[1];
            }

            // Shooting Star: Pay 2 Win
            if (player->extremeGear == ExtremeGear::ShootingStar
            && player->level > 0
            && player->trickCount < 4
            && player->state == Cruise) { // No P2W if Fly/Grind.
                player->speed += pSpeed(60);
                if (player->rings >= 20) {
                    player->rings -= 20;
                    if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x39)); // Ring loss SFX
                } else {
                    TrLInfo->trickAccumulator -= 10;
                }
            }

            // Update the player level now that we have the new amount of tricks
            if (TrLInfo->trickAccumulator >= Tricks4Level[1]) { // Level 3
                player->level = 2;
            } else if (TrLInfo->trickAccumulator >= Tricks4Level[0]) { // Level 2
                player->level = 1;
            } else { // Level 1
                player->level = 0;
            }

            // If the level is different than the one we stored, refill air if level went up and play VFX/SFX.
            if (player->level != startingLevel) {
                if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0xD)); // Levelling SFX
                Player_CreateTrickLevelUpParticles(player);
                if (player->level > startingLevel) {
                    player->currentAir = player->gearStats[player->level].maxAir;
                }
            }
		}

        // Specify that the player is no longer in trick state.
        TrLInfo->isTricking = false;
    }

    if (player->previousState == Death) { // Apply Level again on death.
		if (TrLInfo->trickAccumulator >= Tricks4Level[1]) { // Level 3
			player->level = 2;
		} else if (TrLInfo->trickAccumulator >= Tricks4Level[0]) { // Level 2
			player->level = 1;
		} else { // Level 1
			player->level = 0;
		}
		player->previousState = Cruise;
	}
}