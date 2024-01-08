#include "fastest.hpp"

constexpr std::array<f32, 3> Fastest_SuperCruiseSpeeds = {
        // level 1      level 2      level 3
        pSpeed(245), pSpeed(255), pSpeed(265),// normal
};

void Player_Fastest(Player *player) {
	if(player->extremeGear != ExtremeGear::Fastest) { return; }

	if(player->currentAir > 0 && player->state != Run && player->input->holdFaceButtons.hasAny(BButton, XButton)) {
		if(player->state != Cruise) { return; }
		const bool isLateBooster = player->characterArchetype == LateBooster;
		const bool isCombat = player->characterArchetype == CombatArchetype;
		if(player->input->toggleFaceButtons.hasAny(BButton, XButton)) {
			player->currentAir -= 4000;
		}

		const bool isBerserker = player->fastest_timer >= 90;
		if(!isBerserker) { player->fastest_timer += 1; }

		player->fastest_superCruise = true;
		const f32 newSpeed = player->speed + 0.0027006172839506F;// per frame

		const auto isBoostArchetype = player->characterArchetype == BoostArchetype ? 1U : 0U;
		const f32 cruiseSpeed = Fastest_SuperCruiseSpeeds[player->level];
		// cruiseSpeed = Fastest_SuperCruiseSpeeds[(isCombat | isLateBooster)][player->level];
		if(newSpeed < cruiseSpeed) { player->speed = newSpeed; }
		player->speedCap = cruiseSpeed;
		player->currentAir -= 320;

		if(isBerserker) { player->specialFlags |= berserkerEffect; }
	} else {
		player->fastest_superCruise = FALSE;
		player->fastest_timer = 0;
		player->specialFlags &= ~berserkerEffect;
		if(player->state == Cruise) {
			player->unkF40 = 0;
			player->speedCap = player->gearStats[player->level].topSpeed;
		}
	}
}