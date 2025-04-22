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

    switch (player->character) {
        case Sonic:
            customTexture = DumpFile("ZSKS", 0);
            break;
        case Tails:
            customTexture = DumpFile("ZSKT", 0);
            break;
        case Knuckles:
            customTexture = DumpFile("ZSKK", 0);
            break;
        case Amy:
            customTexture = DumpFile("ZSKA", 0);
            break;
        case Shadow:
            customTexture = DumpFile("ZSKD", 0);
            break;
        case Nights:
            customTexture = DumpFile("ZSK0", 0);
            break;
        default:
            break;
    }

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
                    customTexture = (u8*)customTexture + 0x8010;
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
        case Nights: {
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
        default:
            break;
    }

    return customTexture;

}

ASMUsed void InitializeTextureHotswap() {
    tex_HotswapTextures = DumpFile("POT", 1);
}