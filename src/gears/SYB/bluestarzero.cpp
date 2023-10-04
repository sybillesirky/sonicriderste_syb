#include "bluestarzero.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

BSZInfo PlayerBSZInfo[8];

void Player_BSZ(Player *player) {
    if (player->extremeGear != BlueStarZero) return;

    BSZInfo *BSZInfo = &PlayerBSZInfo[player->index];

    if (player->input->holdFaceButtons & BButton) {
        if (player->state == Jump || player->state == Fall) {
            BSZInfo->GravDiveState = true;
            player->speed = pSpeed(250);

            if (player->input->holdFaceButtons & LStickDown) {
                BSZInfo->verticalVelocity += 0.02;
                if (BSZInfo->verticalVelocity > 0.5) {
                    BSZInfo->verticalVelocity = 0.5;
                }
            }
            else if (player->input->holdFaceButtons & LStickUp) {
                BSZInfo->verticalVelocity -= 0.02;
                if (BSZInfo->verticalVelocity < -0.5) {
                    BSZInfo->verticalVelocity = -0.5;
                }
            }
            else {
                BSZInfo->verticalVelocity = BSZInfo->verticalVelocity / 1.1;
            }

            player->verticalSpeed = BSZInfo->verticalVelocity;
        }
    }else{
        BSZInfo->GravDiveState = false;
        BSZInfo->verticalVelocity = 0;
    }
}