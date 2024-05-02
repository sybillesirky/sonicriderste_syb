#include "virtuoso.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

std::array<VirtuosoInfo, MaxPlayerCount> PlayerVirtuosoInfo;

void Player_Virtuoso_PlayMusic(Player *player) {
    if (player->input->toggleFaceButtons & AButton) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x00)); // C
    }
    else if (player->input->toggleFaceButtons & BButton) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x01)); // C Sharp
    }
}

void Player_Virtuoso(Player *player) {

    VirtuosoInfo *VirtInfo = &PlayerVirtuosoInfo[player->index];

    if (VirtInfo->performanceMode == true && player->state != Cruise) {
        VirtInfo->performanceMode = false;
    }

    if (player->input->toggleFaceButtons & XButton && player->state == Cruise) {
        VirtInfo->performanceMode = true;
    }

    if (VirtInfo->performanceMode == true) {
        player->aiControl = true;
        Player_Virtuoso_PlayMusic(player);
    }
    else {
        player->aiControl = false;
    }

    player->rings = VirtInfo->performanceMode;
}