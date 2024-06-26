#pragma once

#include "containers/vector3.hpp"
#include "globalDefs.hpp"
#include "riders/character.hpp"
#include "riders/hud.hpp"
#include "riders/player.hpp"

ASMDefined Vector3F gcosNnSystemVecZeroFast;
ASMDefined void* gpsTexList_Particle;

ASMDefined std::array<Matrix3x3F *, MaxPlayerCount> const gpsaMtxList_Player;
ASMDefined void nnMultiplyMatrix(Matrix3x3F *A, Matrix3x3F *B, Matrix3x3F &out);
ASMDefined void nnMakeTranslateMatrix(Matrix3x3F *M, f32 x, f32 y, f32 z);
ASMDefined void InitTextureHook(void *, u32);
ASMDefined void HookTexture(u32);
ASMDefined void CreateHUDElement(u32 x, u32 y, u32 xLength, u32 yLength, u32 rgba, u32, void *textureBoundaries);

ASMDefined void RenderText(
		s32 x,
		s32 y,
		u32 rgba,
		u32 textLength,
		u32 lineSpacing,
		u32 textID,
		void *textTable,
		void *textureStruct,
		f32);

ASMDefined u32 CenterText(u32 textLength, u32 lineSpacing, u32 textID, void *textTable);
ASMDefined void utilBinSetUpMotion(void* savePtr, void* gnmFile);

struct AttackObjReadManager{
	u8 motptrsID;
	u8 character;
};

ASMDefined std::array<AttackObjReadManager, 8> gsAttackObjReadManager;

struct AnimationIDInfo {
    std::array<u8*, 5> gearTypePtrs;
};

ASMDefined AnimationIDInfo lbl_001F1140[];
ASMDefined u16 tu16ExclusiveMotionNo;

ASMDefined std::array<void*, 0x1000> gpsaMotion_PlayerNeo;

constexpr std::array<u8, TotalCharacterAmount+1> CharacterAnimationFiles = {
		0x0,
		0x1,
		0x2,
		0x3,
		0x4,
		0x5,
		0x6,
		0x7,
		0x8,
		0x9,
		0xA,
		0xB,
		0xC,
		0xD,
		0xE,
		0xF,
		0x10,
		0xA, // silver
		0x0, // metal sonic
		0x1, // emerl
		0x6, // blaze
		0x2, // chaos
		0x8, // tikal
		0xA, // variable character
};

void Custom_CreateHUDElement(void *textureStructs, u32 textureID, u32 textureRGBA, HUDStruct *hud);
void GetPlayerPositionRotationMatrix(Player &player, Matrix3x3F &out);
ASMUsed u32 SetupCustomAnimationArchive(u8 *packManStart, u32 *packManOffsetStart, u32 playerIndex);
Vector3F MatrixExtractTranslation(Matrix3x3F *m);

struct TexInfo {
    u32 unk0;
    char *filename;
    u16 unk8;
    u16 unkA;
    u32 unkC;
    u32 unk10;
};
static_assert(sizeof(TexInfo) == 0x14);

// TODO: document this struct
struct TexStruct {
    fillerData<0x40> data;
};

struct TexStructHeader {
    u32 texNum;
    TexStruct *texStart;
};

constexpr TexInfo defaultTexInfo = {
        0, nullptr, 1, 1, 0, 0
};

ASMDefined u32 InitTextureStruct(TexStruct *tex, const TexInfo *info = &defaultTexInfo, const void *gvrTexture = nullptr);

/// estimates tex list size
ASMDefined u32 func_80053E38(u32 texNum);

/// sets up tex structs and saves a pointer to it
ASMDefined void func_80053D20(void *savePtr, u32 texNum, TexStructHeader *texStructs);

// getset_gno
ASMDefined void lbl_000478A0(void *packManStart, void *packManOffsetStart, void *savePtr);

struct GVRTextureArchive {
    u16 texNum;
    u16 flag; // either 0 or 1
    u32 offsetTable[];
};

struct GVRTexture {
    std::array<char, 4> gcixMagic;
    fillerData<0xC> data1;
    std::array<char, 4> gvrtMagic;
    u32 texSize; ///< (little endian) starting from the currently read offset, so this variable is 0x18 bytes less than the entire file
    fillerData<0x4> data2;
    u16 width;
    u16 height;
    u8 *texData;
};