#pragma once

#include "containers/graphicalobject.hpp"
#include "riders/player.hpp"

extern std::array<u16, MaxPlayerCount> PlayerDefaultIDs;

constexpr m2darray<u32, TotalCharacterAmount, 2> CharacterDefaultGearData = {{
    {0x0086, 0x0000},   // Sonic
    {0x0087, 0x0001},   // Tails
    {0x0088, 0x0002},   // Knuckles
    {0x0089, 0x0003},   // Amy
    {0x008A, 0x0004},   // Jet
    {0x008B, 0x0005},   // Storm
    {0x008C, 0x0006},   // Wave
    {0x0, 0x0},        // Eggman placeholder
    {0x008E, 0x0008},   // Cream
    {0x008D, 0x0007},   // Rouge
    {0x00CE, 0x00FB},   // Shadow
    {0x0, 0x0},         // Super Sonic placeholder
    {0x00A6, 0x0020},   // NiGHTS
    {0x00A7, 0x0021},   // Aiai
    {0x00AA, 0x0024},   // Ulala
    {0x00A8, 0x0022},   // E10G
    {0x00A9, 0x0023},   // E10B
    {0x00E7, 0x00E1},   // Silver
    {0x00E8, 0x00E2},   // Metal Sonic
    {0x00E9, 0x00E3},   // Emerl
    {0x00EA, 0x00E4},   // Blaze
    {0x00EB, 0x00E5},   // Chaos
    {0x00EC, 0x00E6},   // Tikal
}};

ASMUsed void RotateNextDefaultGear(GraphicalObject *object);
ASMUsed void InitFirstDefaultGear(Player *player);