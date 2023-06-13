#include "archangel.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

// CONCEPT: ARCHANGEL
// A Gear that starts off mediocre and doesn't level up (stuck at level 2), but can change its properties
// through what I want to call an "offering", either to the Angel or to the Devil (said outcome is random).
// ANGEL: Go down to Level 1, but your speed increases greatly.
// DEVIL: Go up to level 3, but you boost cost increases greatly.
// Your mode lasts for as long as you have Rings. Rings go down by 1 per second (60 frames). (THAT IS SUBJECT TO CHANGE.)

// We store "transformed or not" in player->genericBool.
// We store the Ring Drain timer in player->genericCounter1.
// We store the current mode in player->genericCounter2 so we can set the exhaust colours.

void Player_CreateArchAngelParticles(Player *player) {
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

constexpr GearLevelStats Genocide = { // Internally Level 3
		200000, // max air
		64, // air drain
		0x0000014D, // drift cost
		0x4E20, // boost cost
		0x9C40, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(240) // boost speed
};

constexpr GearLevelStats Neutral = { // Internally Level 2
		150000, // max air
		16, // air drain
		0x000000FA, // drift cost
		0x4E20, // boost cost
		0x7530, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(220) // boost speed
};

constexpr GearLevelStats Pacifist = { // Internally Level 1
		200000, // max air
		64, // air drain
		0x0000014D, // drift cost
		0x4E20, // boost cost
		0x61A8, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(260) // boost speed
};

void Player_ArchAngel_LevelUpdater(Player *player, const GearLevelStats *stats, int inputLevel) {
    player->gearStats[inputLevel].maxAir = stats->maxAir;
    player->gearStats[inputLevel].airDrain = stats->passiveAirDrain;
    player->gearStats[inputLevel].driftCost = stats->driftingAirCost;
    player->gearStats[inputLevel].boostCost = stats->boostCost;
    player->gearStats[inputLevel].tornadoCost = stats->tornadoCost;
    player->gearStats[inputLevel].boostSpeed = stats->boostSpeed;
    if (player->characterArchetype == BoostArchetype) {
		 player->gearStats[inputLevel].boostSpeed += BoostArchetypeBoostSpeeds[inputLevel];
	}
}

void Player_ArchAngel_SetStats(Player *player) {
    if (player->specialFlags != (noSpecialFlags)) { // ArchAngel should never have special flags at Start Line, so this check is fine.
        player->specialFlags = (noSpecialFlags);
        Player_ArchAngel_LevelUpdater(player, &Pacifist, 0);
        Player_ArchAngel_LevelUpdater(player, &Neutral, 1);
        Player_ArchAngel_LevelUpdater(player, &Genocide, 2);
    }
}

void Player_ArchAngel_Detransform(Player *player) {
    player->genericBool = false;
    if (player->level == 2) {
        player->unkBAC ^= 0x0100; //Handedness Swap!
    }
    player->level = 1;
    player->genericCounter2 = 0;
    player->specialFlags = (noSpecialFlags);
    if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0xD)); // Levelling SFX
    Player_CreateArchAngelParticles(player);
}

void Player_ArchAngel(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBArchAngelEXLoad) return;
	if (player->extremeGear != DefaultGear) return;

	if (player->input->toggleFaceButtons & DPadUp) {
        if (player->genericBool == true) { // Manual Detransform.
            Player_ArchAngel_Detransform(player);
            return;
        }
        if (player->rings <= 10) {
            return;
        }
        if (lbl_RNG_Number(2) == 0) { // Pacifist Mode
            if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x40));
            player->specialFlags |= noSpeedLossChargingJump;
            player->level = 0;
            player->genericBool = true;
        }
        else { // Genocide Mode
            if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x1C));
            player->specialFlags |= berserkerEffect;
            player->specialFlags |= noPits;
            player->specialFlags |= noSpeedLossTurning;
            player->level = 2;
            player->unkBAC ^= 0x0100; //Handedness Swap!
            player->genericBool = true;
        }
        Player_CreateArchAngelParticles(player);
        player->rings -= 10;
    }

    if (player->state == StartLine) { // Initialising behaviours.
        Player_ArchAngel_SetStats(player);
        player->level = 1;
        player->genericCounter2 = 0;
        player->genericBool = false; // Not in transformed state in the beginning.
        player->genericCounter1 = 20; // Initialise Ring Drain counter so we don't have to later.
	}

    // We handle the Ring Drain while transformed here.
    if (player->genericBool == true) {
        player->genericCounter1 -= 1;
        if (player->genericCounter1 == 0) {
            player->rings -= 1;
            player->genericCounter1 = 20;
            if (player->rings == 0) { // Detransform Behaviours go here.
                Player_ArchAngel_Detransform(player);
            }
        }
    }

    // Angel Mode exhaust & punishment checker.
    if (player->level == 0) {
        player->genericCounter2 = 1;
        if (player->state == AttackingPlayer || player->state == AttackedByPlayer) {
            player->rings = 0;
            Player_ArchAngel_Detransform(player);
            if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x0F)); // Superbad Buzz
        }
    }

    // Devil Mode exhaust & punishment checker.
    if (player->level == 2) {
        player->genericCounter2 = 2;
        if (player->state == Cruise && player->speed < pSpeed(150)) {
            player->rings = 0;
            Player_ArchAngel_Detransform(player);
            if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x0F)); // Superbad Buzz
        }
    }
}