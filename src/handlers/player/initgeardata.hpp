#pragma once

#include "riders/character.hpp"

ASMDefined f32 gf32_SystemHzAdd1;

struct LevelTypeStats {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
};

struct TypeStats {
	std::array<LevelTypeStats, 3> stats;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ f32 unk58;
	/* 0x5C */ f32 unk5C;
	/* 0x60 */ f32 unk60;
};

constexpr std::array<TypeStats, 3> StatPresets = {{
		{{{{0.462962955236435, 0.717592597007751, 0.75, 0.00385802472010255, 0.000848765426781028, 0.000308641989249736, 50}, {0.462962955236435, 0.717592597007751, 0.810185194015503, 0.00385802472010255, 0.000848765426781028, 0.000308641989249736, 70}, {0.532407402992249, 0.736111104488373, 0.870370388031006, 0.00441358052194119, 0.000563271576538682, 0.000540123437531292, 70}}}, 0.800000011920929, 0.0253072753548622, 0.00959931127727032, 0.00261799385771155},
		{{{{0.648148119449615, 0.671296298503876, 0.717592597007751, 0.00555555615574121, 0.000254629616392776, 0.000123456789879128, 40}, {0.648148119449615, 0.671296298503876, 0.777777791023254, 0.00555555615574121, 0.000794753083027899, 0.000123456789879128, 60}, {0.671296298503876, 0.717592597007751, 0.837962985038757, 0.00624999590218067, 0.00115740741603076, 0.000231481477385387, 60}}}, 0.899999976158142, 0.0253072753548622, 0.00959931127727032, 0.00261799385771155},
		{{{{0.370370358228683, 0.671296298503876, 0.731481492519379, 0.0030864195432514, 0.000895061704795808, 0.000578703708015382, 80}, {0.462962955236435, 0.671296298503876, 0.791666686534882, 0.00385802472010255, 0.000640432117506862, 0.000192901221453212, 80}, {0.462962955236435, 0.671296298503876, 0.851851880550385, 0.00385802472010255, 0.000640432117506862, 0.000578703708015382, 90}}}, 0.800000011920929, 0.0253072753548622, 0.00959931127727032, 0.00261799385771155},
}};

constexpr std::array<f32, TotalCharacterAmount> CharacterWeights = {
		1, 		// Sonic
		0.9F,	// Tails
		1.35F,	// Knuckles
		1.15F,	// Amy
		0.95F,	// Jet
		1.25F,	// Storm
		1.05F,	// Wave
		1.3F,	// Eggman
		0.9F,	// Cream
		1.15F,	// Rouge
		1.05F,	// Shadow
		0.75F,	// Super Sonic
		0.85F,	// NiGHTS
		1.1F,	// AiAi
		1.15F,	// Ulala
		1.25F,	// E10G
		1.2F,	// E10B
		0.95F,	// Silver
		1.2F,	// Metal Sonic
		1.15F,	// Emerl
		1,		// Blaze
		1.05F,	// Chaos
		1.15F,	// Tikal
};

constexpr std::array<f32, 3> BoostArchetypeBoostSpeeds = {
		pSpeed(1.1), pSpeed(2.1), pSpeed(3.1)
};

constexpr std::array<f32, 3> EmerlTopSpeedBonus = {
		pSpeed(20), pSpeed(15), pSpeed(5)
};

constexpr std::array<f32, ArchetypeCount> Archetype_TopSpeedBonus = {
		pSpeed(5.0f), // all rounder (+5)
		pSpeed(0), // late booster
		pSpeed(10.1f), // top speed (+10)
		pSpeed(0), // combat
		pSpeed(0), // drift
		pSpeed(14.0f), // no type (+14)
		pSpeed(0), // super sonic
		pSpeed(0), // boost
		pSpeed(0), // trickster
		pSpeed(0), // none archetype
        pSpeed(0), // Windcatcher
        pSpeed(0), // Turning
        pSpeed(0), // Mechanic
		pSpeed(0), // Attuned
		pSpeed(0), // Collector
};