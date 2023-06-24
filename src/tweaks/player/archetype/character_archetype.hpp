#pragma once

#include "context.hpp"

constexpr CharacterArchetypes AllCharacterArchetypes[] = {
		LateBooster, // sonic
		LateBooster, // tails
		CombatArchetype, // knuckles
		DriftArchetype, // amy
		TopSpeedArchetype, // jet
		TopSpeedArchetype, // storm
		CombatArchetype, // wave
		AllRounder, // eggman
		AllRounder, // cream
		DriftArchetype, // rouge
		CombatArchetype, // shadow SYB: Popular demand change.
		SuperSonicArchetype, // super sonic
		TopSpeedArchetype, // nights
		LateBooster, // aiai
		CombatArchetype, // ulala
		NoTypeArchetype, // e10g
		DriftArchetype, // e10b/e10r
		TricksterArchetype, // silver
		BoostArchetype, // metal sonic
		AllRounder, // emerl
		TricksterArchetype, // blaze
		BoostArchetype, // chaos
		TricksterArchetype, // tikal
};

ASMUsed void Player_CharacterArchetype(Player *player);