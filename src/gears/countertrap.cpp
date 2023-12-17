#include "countertrap.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"
#include "handlers/player/initgeardata.hpp"

constexpr f32 TrapGear_BoostSpeeds[] = {pSpeed(245.0f), pSpeed(275.0f), pSpeed(305.0f)};
u32 TrapFrameCounter = 0;


void Player_CounterTrap(Player *player) {
    EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
    if (exLoads.gearExLoadID != TrapEXLoad) return;

    if (player->characterArchetype == BoostArchetype) { 
        player->gearStats[player->level].boostSpeed = TrapGear_BoostSpeeds[player->level] + BoostArchetypeBoostSpeeds[player->level];
	} else { player->gearStats[player->level].boostSpeed = TrapGear_BoostSpeeds[player->level];}
    
    if(player->characterArchetype == LateBooster){
		player->gearStats[player->level].tornadoCost = (player->gearptr->levelStats[player->level].tornadoCost * 80) / 100;
	}

    if (player->state == StartLine) {
        TrapFrameCounter = 0;
    }

    // //Solemn Strike    
    // //if X button pressed, activate berserker veil for 2.5 seconds/150 frames
    // //Conditions for activation: Press X. Berserker effect not active. Cruise state. Enough air to pay the 3.75x tornado cost.
    // s32 strikeCost;
    // u32 strikeLength;
    // if(player->characterArchetype == CombatArchetype) {
    //     strikeCost = (player->gearptr->levelStats[player->level].tornadoCost * 325) / 100;
    //     strikeLength = 180;
    // } else {
    //     strikeCost = (player->gearptr->levelStats[player->level].tornadoCost * 375) / 100;
    //     strikeLength = 150;
    // }

    // if(player->input->toggleFaceButtons & XButton && !(player->specialFlags & berserkerEffect) && player->state == Cruise && player->currentAir >= strikeCost) {
    //     player->specialFlags |= (berserkerEffect);
    //     player->currentAir -= strikeCost;
    //     TrapFrameCounter = 0;
    //     player->gearStats[player->level].topSpeed += pSpeed(20.0f);
    //     PlayAudioFromDAT(Sound::SFX::RailHoopDash);
    // }
    
    u32 atkLength;
    if(player->characterArchetype == CombatArchetype){
		atkLength = 15;
    } else {
        atkLength = 10;
    }

    if (player->unk1040) {
        TrapFrameCounter = 0;
        if (player->specialFlags & (~berserkerEffect)) {
            player->specialFlags |= (berserkerEffect);
        }
        // PlayAudioFromDAT(Sound::SFX::RailHoopDash);
        // player->unk1040 = 0;
    }

    //If counter trap has been activated, berserker effect should be active, increased speed granted.
    //If the frame counter hits 150, turn berserker effect off and set the frame counter back to 0.
    //Otherwise, keep incrementing the Frame counter.
    if (player->specialFlags & berserkerEffect) {
        if (TrapFrameCounter == atkLength) {
            player->specialFlags &= (~berserkerEffect);
            // player->gearStats[player->level].topSpeed -= pSpeed(20.0f);
            player->unkF40 = 0;
            // player->unk1040 = 0;
        } else {
            // if (player->state == Cruise) player->speedCap = player->gearStats[player->level].topSpeed;
            TrapFrameCounter++;
        }
    }
}