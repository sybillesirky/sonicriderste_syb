#include "greed.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

void Player_Greed(Player *player) {

    // Lap levelling
    if ((player->currentLap - 1) > player->level && player->currentLap <= 3) {
        player->level = player->currentLap - 1;
    }

    // Additive Boost Speed based on Rings
}