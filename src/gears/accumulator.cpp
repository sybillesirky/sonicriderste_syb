#include "accumulator.hpp"
#include "handlers/player/initgeardata.hpp"
#include "lib/sound.hpp"
#include "blastGaugeGears.hpp"

std::array<AccumulatorInfo, MaxPlayerCount> PlayerAccumulatorInfo;

void Player_Accumulator(Player *player) {

    AccumulatorInfo *AccumInfo = &PlayerAccumulatorInfo[player->index];
    BlastGaugeInfo *bgInfo = &PlayerBlastGaugeInfo[player->index];

    if (player->state == StartLine) {
        AccumInfo->collectedSpeed = 0;
        AccumInfo->dashPanelCooldown = 0;
    }

    if (player->currentLap <= 3 && player->currentLap != 0) {
        player->level = player->currentLap - 1;
    }

    bgInfo->currentGauge = AccumInfo->collectedSpeed * 2000;
}