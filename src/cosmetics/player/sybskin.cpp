#include "sybskin.hpp"
#include "cosmetics/player/exloads.hpp"

std::array<u8, MaxPlayerCount> PlayerSkinIDs;

ASMUsed void SetPlayerSkin(Player *player) {
    u8* playerSkinID = &PlayerSkinIDs[player->index];
    
    if (player->character == E10R ||
        player->character == E10G
    ) {
        *playerSkinID = 0;
        return;
    }

    if(player->input->holdFaceButtons.hasAny(XButton)) {
        *playerSkinID = 1;
    }
    else if(player->input->holdFaceButtons.hasAny(YButton)) {
        *playerSkinID = 2;
    }
    else {
        *playerSkinID = 0;
    }
}