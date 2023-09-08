#include "angryarnold.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

AngryArnoldInfo PlayerAngryArnoldInfo[8];

constexpr GearLevelStats Level5 = {
		200000, // max air
		64, // air drain
		100, // drift cost
		0x9C40, // boost cost
		0x9C40, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(260) // boost speed
};

constexpr GearLevelStats Level4 = {
		200000, // max air
		64, // air drain
		100, // drift cost
		0x9C40, // boost cost
		0x9C40, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(245) // boost speed
};

constexpr GearLevelStats Level3 = {
		150000, // max air
		32, // air drain
		100, // drift cost
		0x4E20, // boost cost
		0x7530, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(230) // boost speed
};

constexpr GearLevelStats Level2 = {
		150000, // max air
		32, // air drain
		100, // drift cost
		0x4E20, // boost cost
		0x7530, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(215) // boost speed
};

constexpr GearLevelStats Level1 = {
		100000, // max air
		16, // air drain
		100, // drift cost
		0x3A98, // boost cost
		0x61A8, // tornado cost
		pSpeed(100), // drift dash speed, unused
		pSpeed(200) // boost speed
};

void Player_AngryArnold_LevelDeterminer(Player *player) {

    AngryArnoldInfo *AAInfo = &PlayerAngryArnoldInfo[player->index];

    if (player->rings < 20) {
        AAInfo->CurrentArnoldLevel = 0;
    } else if (player->rings < 40) {
        AAInfo->CurrentArnoldLevel = 1;    
    } else if (player->rings < 60) {
        AAInfo->CurrentArnoldLevel = 2;    
    } else if (player->rings < 80) {
        AAInfo->CurrentArnoldLevel = 3;
    } else {
        AAInfo->CurrentArnoldLevel = 4;
    }
}

void Player_AngryArnold_StatSetter(Player *player, const GearLevelStats *stats) {
    player->gearStats[player->level].maxAir = stats->maxAir;
    player->gearStats[player->level].airDrain = stats->passiveAirDrain;
    player->gearStats[player->level].driftCost = stats->driftingAirCost;
    player->gearStats[player->level].boostCost = stats->boostCost;
    player->gearStats[player->level].tornadoCost = stats->tornadoCost;
    player->gearStats[player->level].boostSpeed = stats->boostSpeed;
	if (player->characterArchetype == BoostArchetype) {
		player->gearStats[player->level].boostSpeed += BoostArchetypeBoostSpeeds[player->level];
	}
}

void Player_AngryArnold_SetStats(Player *player) {
    if (player->specialFlags != (noSpecialFlags)) {
        player->specialFlags = (noSpecialFlags);
        Player_AngryArnold_StatSetter(player, &Level1);
    }
}

void Player_CreateAngryArnoldParticles(Player *player) {
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

void Player_AngryArnold(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBAngryArnoldEXLoad) return;
	if (player->extremeGear != PowerfulGear) return;

    AngryArnoldInfo *AAInfo = &PlayerAngryArnoldInfo[player->index];

    if (player->state == StartLine) { // Initialising behaviours.
		AAInfo->LatestArnoldLevel = 0;
        AAInfo->CurrentArnoldLevel = 0;
        Player_AngryArnold_SetStats(player);
	}

    Player_AngryArnold_LevelDeterminer(player);

    if (AAInfo->CurrentArnoldLevel != AAInfo->LatestArnoldLevel) {
        switch(AAInfo->CurrentArnoldLevel) {
            case 4:
                player->level = 2;
                Player_AngryArnold_StatSetter(player, &Level5);
                break;
            case 3:
                player->level = 2;
                Player_AngryArnold_StatSetter(player, &Level4);
                break;
            case 2:
                player->level = 1;
                Player_AngryArnold_StatSetter(player, &Level3);
                break;
            case 1:
                player->level = 1;
                Player_AngryArnold_StatSetter(player, &Level2);
                break;
            default:
                player->level = 0;
                Player_AngryArnold_StatSetter(player, &Level1);
        }

        if (AAInfo->CurrentArnoldLevel < AAInfo->LatestArnoldLevel) {
            player->currentAir = player->gearStats[player->level].maxAir;
        }
		Player_CreateAngryArnoldParticles(player);
    }

    AAInfo->LatestArnoldLevel = AAInfo->CurrentArnoldLevel;
}