#include "greed.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"

void Player_Greed(Player *player) {

    // Lap levelling
    if ((player->currentLap - 1) > player->level && player->currentLap <= 3) {
        player->level = player->currentLap - 1;
    }

    // Additive Boost Speed based on Rings
    player->gearStats[player->level].boostSpeed = Gears[ExtremeGear::Greed].levelStats[player->level].boostSpeed + pSpeed(player->rings / 2);
}