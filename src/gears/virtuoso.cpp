#include "virtuoso.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

std::array<VirtuosoInfo, MaxPlayerCount> PlayerVirtuosoInfo;

void Player_Virtuoso_PlayMusic(Player *player) {

    VirtuosoInfo *VirtInfo = &PlayerVirtuosoInfo[player->index];

    u8 playedNote = 0;
    if (player->input->toggleFaceButtons & LStickUp) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x00)); // C
        playedNote = 1;
    }
    else if (player->input->toggleFaceButtons & LStickRight) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x01)); // C Sharp
        playedNote = 2;
    }
    else if (player->input->toggleFaceButtons & LStickDown) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x02)); // D
        playedNote = 3;
    }
    else if (player->input->toggleFaceButtons & LStickLeft) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x03)); // D Sharp
        playedNote = 4;
    }
    else if (player->input->toggleFaceButtons & DPadUp) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x04)); // E
        playedNote = 5;
    }
    else if (player->input->toggleFaceButtons & DPadRight) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x05)); // F
        playedNote = 6;
    }
    else if (player->input->toggleFaceButtons & DPadDown) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x06)); // F Sharp
        playedNote = 7;
    }
    else if (player->input->toggleFaceButtons & DPadLeft) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x07)); // G
        playedNote = 8;
    }
    else if (player->input->toggleFaceButtons & AButton) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x08)); // G Sharp
        playedNote = 9;
    }
    else if (player->input->toggleFaceButtons & BButton) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x09)); // A
        playedNote = 10;
    }
    else if (player->input->toggleFaceButtons & YButton) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x0A)); // A Sharp
        playedNote = 11;
    }
    else if (player->input->toggleFaceButtons & ZButton) {
        PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::GearVirtuoso, 0x0B)); // B
        playedNote = 12;
    }
    
    if (playedNote != 0) {
        VirtInfo->timeSinceLastNote = 0;
        VirtInfo->totalNotesPlayed += 1;
    }
}

void Player_Virtuoso(Player *player) {

    VirtuosoInfo *VirtInfo = &PlayerVirtuosoInfo[player->index];

    if (VirtInfo->performanceMode == true && player->state != Cruise) {
        VirtInfo->performanceMode = false;
    }

    if (player->input->toggleFaceButtons & XButton && player->state == Cruise && VirtInfo->performanceMode == false) {
        VirtInfo->performanceMode = true;
    }

    if (player->input->toggleFaceButtons & XButton && player->state == Cruise && VirtInfo->performanceMode == true) {
        //Player_Virtuoso_ExitPerformance(player);

        VirtInfo->performanceMode = false;
    }

    if (VirtInfo->performanceMode == true) {
        player->aiControl = true;
        Player_Virtuoso_PlayMusic(player);
    }
    else {
        player->aiControl = false;
        VirtInfo->performanceDuration = 0;
        VirtInfo->totalNotesPlayed = 0;
        VirtInfo->timeSinceLastNote = 0;
    }

    player->rings = VirtInfo->performanceMode;
}