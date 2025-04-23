#include "cosmetics/player/exloads.hpp"
#include "filehandler_dat.hpp"
#include "cosmetics/player/sybskin.hpp"

ASMDefined void* tex_HotswapTextures;

ASMUsed void* TextureHotswapHandler(Player *player, void* gvrTexture, u32 textureID) {
    u32 exLoadIndex;
    // if (player->character != E10R) return gvrTexture;
    // if (!customTexture) return gvrTexture;

    u8* playerSkinID = &PlayerSkinIDs[player->index];

    exLoadIndex = Player_EXLoadData[player->input->port][CharacterEXLoadMode].exLoadIndex;

    if (player->character == E10R) {
        void* customTexture = tex_HotswapTextures;
        if (CharacterEXLoadDataSlots[exLoadIndex].exLoadID == E10REXLoad) {
            customTexture = (u8*)customTexture + 0x2E40; // skip to e10r textures
        }
        else if (CharacterEXLoadDataSlots[exLoadIndex].exLoadID == E10YEXLoad) {
            customTexture = (u8*)customTexture + 0x5C80; // SYB: skip to e10y textures
        }

        if (textureID == 2) {
            gvrTexture = customTexture;
        } else if (textureID == 1) {
            gvrTexture = (u8*)customTexture + 0x2B20;
        }
    
        return gvrTexture;
    }

    if (*playerSkinID == 0) {
        return gvrTexture;
    }

    if (CharacterEXLoadDataSlots[exLoadIndex].exLoadID != NoneEXLoad) {
        return gvrTexture;
    }

    // Skin system code from here

    void* customTexture = gvrTexture;

    const Character &character = Characters[player->character];
    std::string skinFilename = "ZSK";
    skinFilename += character.model;
    skinFilename += std::to_string(*playerSkinID);
    customTexture = DumpFile(skinFilename.c_str(), 0);

    switch (player->character) {
        case Sonic:
        case Tails:
        case Knuckles: {
            switch (textureID) {
                case 1:
                    if (player->extremeGear >= ExtremeGear::Darkness &&
                        player->extremeGear < ExtremeGear::GunGear) {
                        customTexture = gvrTexture;
                        break;
                    }
                    customTexture = (u8*)customTexture + 0x2020;
                    break;
                case 2:
                    customTexture = (u8*)customTexture;
                    break;
                case 3:
                    customTexture = (u8*)customTexture;
                    break;
                default:
                    customTexture = gvrTexture;
                    break;
            }
            break;   
        }
        case Amy:
        case Shadow:
        case Nights:
        case Ulala: {
            switch (textureID) {
                case 1:
                    if (player->extremeGear >= ExtremeGear::Darkness &&
                        player->extremeGear < ExtremeGear::GunGear) {
                        customTexture = gvrTexture;
                        break;
                    }
                    customTexture = (u8*)customTexture;
                    break;
                case 2:
                    customTexture = (u8*)customTexture;
                    break;
                case 3:
                    customTexture = (u8*)customTexture;
                    break;
                default:
                    customTexture = gvrTexture;
                    break;
            }
            break;   
        }
        // Jet is weird because his MAIN texture (one with eye) is actually 64x64 instead of 128x128.
        // (In comparison, it's the other way around for other characters.)
        case Jet: {
            switch (textureID) {
                case 1:
                    if (player->extremeGear >= ExtremeGear::Darkness &&
                        player->extremeGear < ExtremeGear::GunGear) {
                        customTexture = gvrTexture;
                        break;
                    }
                    customTexture = (u8*)customTexture;
                    break;
                case 2:
                    if (player->extremeGear >= ExtremeGear::Darkness &&
                        player->extremeGear < ExtremeGear::GunGear) {
                        customTexture = (u8*)customTexture;
                        break;
                    }
                    customTexture = (u8*)customTexture + 0x2020;
                    break;
                case 3:
                    customTexture = (u8*)customTexture + 0x2020;
                    break;
                default:
                    customTexture = gvrTexture;
                    break;
            }
            break;   
        }
        case Wave:
        case Storm: {
            switch (textureID) {
                case 1:
                    if (player->extremeGear >= ExtremeGear::Darkness &&
                        player->extremeGear < ExtremeGear::GunGear) {
                        customTexture = gvrTexture;
                        break;
                    }
                    customTexture = (u8*)customTexture + 0x2020;
                    break;
                case 2:
                    if (player->extremeGear >= ExtremeGear::Darkness &&
                        player->extremeGear < ExtremeGear::GunGear) {
                        customTexture = (u8*)customTexture + 0x2020;
                        break;
                    }
                    customTexture = (u8*)customTexture;
                    break;
                case 3:
                    customTexture = (u8*)customTexture;
                    break;
                case 4:
                    customTexture = (u8*)customTexture;
                    break;
                default:
                    customTexture = gvrTexture;
                    break;
            }
            break;   
        }
        // Eggman doesn't want Skate exceptions, otherwise actually works like Jet does.
        case Eggman: {
            switch (textureID) {
                case 1:
                    customTexture = (u8*)customTexture;
                    break;
                case 2:
                    customTexture = (u8*)customTexture + 0x2020;
                    break;
                case 3:
                    customTexture = (u8*)customTexture + 0x2020;
                    break;
                default:
                    customTexture = gvrTexture;
                    break;
            }
            break;   
        }
        // Silver has a LOT of textures (2 normal, 2 glow). This means he is more complex than anyone else.
        // Order of his textures is as follows:
        // 1. Clothes/Eye emission
        // 2. Clothes/Eye
        // 3. Body
        // 4. Body Emission
        // I'm not including the glasses because FUCK THEM THIS TOOK 2 HOURS AND A CHANGE IN COMPRESSION FORMAT.
        case Silver: {
            switch (textureID) {
                case 1:
                    customTexture = (u8*)customTexture;
                    break;
                case 2:
                    customTexture = (u8*)customTexture + 0x2020;
                    break;
                case 3:
                    customTexture = (u8*)customTexture + 0x4040;
                    break;
                case 4:
                    customTexture = (u8*)customTexture + 0x4860;
                    break;
                default:
                    customTexture = gvrTexture;
                    break;
            }
            break;   
        }
        case MetalSonic:
        case Chaos: {
            switch (textureID) {
                case 1:
                    if (player->extremeGear >= ExtremeGear::Darkness &&
                        player->extremeGear < ExtremeGear::GunGear) {
                        customTexture = gvrTexture;
                        break;
                    }
                    customTexture = (u8*)customTexture;
                    break;
                case 2:
                    customTexture = (u8*)customTexture + 0x2020;
                    break;
                case 3:
                    customTexture = (u8*)customTexture;
                    break;
                default:
                    customTexture = gvrTexture;
                    break;
            }
            break;   
        }
        default:
            break;
    }

    return customTexture;

}

ASMUsed void InitializeTextureHotswap() {
    tex_HotswapTextures = DumpFile("POT", 1);
}