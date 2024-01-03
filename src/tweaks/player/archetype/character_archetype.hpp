#pragma once

#include "riders/character.hpp"
#include "riders/player.hpp"

constexpr std::array<CharacterArchetypes, TotalCharacterAmount> AllCharacterArchetypes = {
		LateBooster, // sonic
		LateBooster, // tails
		CombatArchetype, // knuckles
		Collector, // amy
		TopSpeedArchetype, // jet
		TopSpeedArchetype, // storm
		TopSpeedArchetype, // wave
		AllRounder, // eggman
		AllRounder, // cream
		Collector, // rouge
		CombatArchetype, // shadow
		SuperSonicArchetype, // super sonic
		TopSpeedArchetype, // nights
		LateBooster, // aiai
		TopSpeedArchetype, // ulala
		NoTypeArchetype, // e10g
		AllRounder, // e10b/e10r
		CombatArchetype, // silver
		LateBooster, // metal sonic
		AllRounder, // emerl
		AllRounder, // blaze
		TopSpeedArchetype, // chaos
		CombatArchetype, // tikal
};

ASMUsed void Player_CharacterArchetype(Player *player);