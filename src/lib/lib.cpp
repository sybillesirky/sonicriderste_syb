#include "lib.hpp"
#include "cosmetics/player/exloads.hpp"
#include <array>

void Custom_CreateHUDElement(void* textureStructs, u32 textureID, u32 textureRGBA, HUDStruct *hud) {
    InitTextureHook(textureStructs, textureID);
    HookTexture(3);

	// calculate texture boundaries
	std::array<f32, 4> textureBoundaries = {
			static_cast<f32>(hud->textureX) * hud->scale,
			static_cast<f32>(hud->textureY) * hud->scale,
			static_cast<f32>(hud->textureX + hud->textureXLength) * hud->scale,
			static_cast<f32>(hud->textureY + hud->textureYLength) * hud->scale
	};

    const u32 xLength = hud->screenX + hud->textureXLength;
    const u32 yLength = hud->screenY + hud->textureYLength;

    CreateHUDElement(hud->screenX, hud->screenY, xLength, yLength, textureRGBA, 0, textureBoundaries.data());
}

void GetPlayerPositionRotationMatrix(Player &player, Matrix3x3F &out) {
    auto *mtxList = gpsaMtxList_Player[player.index];
	Matrix3x3F *mtx = mtxList + 2;
    nnMultiplyMatrix(&player.unkC4, mtx, out);
}

constexpr std::array<u8, 35> MikuAnimationMap = {
        0x01,
        0x02,
        0x0A,
        0x0B,
        0x0D,
        0x0E,
        0x0F,
        0x14,
        0x15,
        0x16,
        0x37,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x49,
        0x3E,
        0x3F,
        0x40,
        0x42,
        0x43,
        0x44,
        0x45,
        0x46,
        0x47,
        0x48,
        0x56,
        0x22,
        0x23,
        0x24,
        0x25,
        0x26,
        0x27,
        0x20,
        0xFF,
};

ASMUsed u32 SetupCustomAnimationArchive(u8 *packManStart, u32 *packManOffsetStart, u32 playerIndex) {
    u32 setupFileCount = 0;
    Player &player = players[playerIndex];
    AttackObjReadManager &attackObj = gsAttackObjReadManager[playerIndex];

    const u8 &character = CharacterAnimationFiles[player.character];
    auto gearType = player.gearType;

    if (player.extremeGear == ExtremeGear::OpaOpa) {
        gearType = static_cast<GearType>(3);
        attackObj.motptrsID = (character * 5) + gearType;
        attackObj.character = 0x11;
    }
    else {
        attackObj.motptrsID = (character * 5) + gearType;
        attackObj.character = character;
    }

    u8 *idInfo = lbl_001F1140[character].gearTypePtrs[gearType];

    /*
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(&player, exLoads);

    if (exLoads.characterExLoadID == HatsuneMikuEXLoad) {
        idInfo = MikuAnimationMap.data();
    }
    */

    if (idInfo == nullptr) {
        return setupFileCount;
    }

    const u32 exclusiveMotionIndex = player.gearType * 0x200 + tu16ExclusiveMotionNo;
    u32 *exclusiveMotions = reinterpret_cast<u32*>(&gpsaMotion_PlayerNeo[exclusiveMotionIndex]);

    while (true) {
        const u32 offset = *packManOffsetStart;
        packManOffsetStart++;
        utilBinSetUpMotion(exclusiveMotions, packManStart + offset);

        setupFileCount++;
        exclusiveMotions++;

        const u32 motionIndex = tu16ExclusiveMotionNo;
        tu16ExclusiveMotionNo++;

        const u8 animationID = *idInfo;
        player.animationIDMap[animationID] = motionIndex;

        idInfo++;

        if (*idInfo == 0xFF) {
			break;
		}
    }

    return setupFileCount;
}

Vector3F MatrixExtractTranslation(Matrix3x3F *m) {
    return {m->field[3], m->field[7], m->field[11]};
}