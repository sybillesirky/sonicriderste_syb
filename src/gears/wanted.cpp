#include "wanted.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

std::array<WantedInfo, MaxPlayerCount> PlayerWantedInfo;

constexpr s16 Wanted_BoostSpeeds[5] = {200, 230, 250, 260, 265}; // for each level

// TODO: Fix base stats in the ASM.

void Player_Wanted_LevelDeterminer(Player *player) {
    WantedInfo *WantedInfo = &PlayerWantedInfo[player->index];

    if (player->rings >= 125) {
        WantedInfo->wantedLevel = 4;
    }
    else if (player->rings >= 75) {
        WantedInfo->wantedLevel = 3;
    }
    else if (player->rings >= 50) {
        WantedInfo->wantedLevel = 2;
    }
    else if (player->rings >= 25) {
        WantedInfo->wantedLevel = 1;
    }
    else {
        WantedInfo->wantedLevel = 0;
    }
}

void Player_CreateWantedParticles(Player *player) {
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

void Player_Wanted(Player *player) {
    
    if (player->extremeGear != Wanted) return;
    WantedInfo *WantedInfo = &PlayerWantedInfo[player->index];
    f32 newBoostSpeed;
    Player_Wanted_LevelDeterminer(player);

    // Clear everything at Start Line.
    if (player->state == StartLine) {
        WantedInfo->ringTimer = 0;
        WantedInfo->wantedLevel = 0;
        WantedInfo->lastWantedLevel = 0;
        WantedInfo->storedPlacement = 0;
    }

    // Set level and stats.
    if (WantedInfo->wantedLevel != WantedInfo->lastWantedLevel) {
        switch (WantedInfo->wantedLevel) {
            case 4:
            case 3:
                player->level = 2;
                player->level4 = true;
                break;
            case 2:
                player->level = 2;
                player->level4 = false;
                break;
            case 1:
                player->level = 1;
                player->level4 = false;
                break;
            case 0:
                player->level = 0;
                player->level4 = false;
                break;
            default:
                break;
        }
        player->currentAir = player->gearStats[player->level].maxAir;
        if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0xD)); // Levelling SFX
    Player_CreateWantedParticles(player);
        WantedInfo->storedPlacement = 9; // Forces Boost Speed determiner to trigger, saves lines.
    }

    // Ring gain on 1st Place.
    if (player->placement == 0 && player->state != StartLine) {
        WantedInfo->ringTimer += 1;
        if (WantedInfo->ringTimer == 120) {
            player->rings += 1;
            if (player->rings < 125) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x38)); // Ring SFX
            WantedInfo->ringTimer = 0;
        }
    }

    // Determine the desired Boost speed.
    if (player->placement != WantedInfo->storedPlacement) {
        if (player->placement == 0) {
            newBoostSpeed = Wanted_BoostSpeeds[WantedInfo->wantedLevel] - 10;
            player->gearStats[player->level].boostSpeed = pSpeed(newBoostSpeed);
            player->gearStats[player->level].airDrain = 15;
        }
        else {
            newBoostSpeed = Wanted_BoostSpeeds[WantedInfo->wantedLevel];
            player->gearStats[player->level].boostSpeed = pSpeed(newBoostSpeed);
            player->gearStats[player->level].airDrain = 5;
        }
    }

    WantedInfo->storedPlacement = player->placement;
    WantedInfo->lastWantedLevel = WantedInfo->wantedLevel;
}