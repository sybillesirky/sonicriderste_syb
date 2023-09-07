#include "angryarnold.hpp"
#include "handlers/player/initgeardata.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/sound.hpp"

AngryArnoldInfo PlayerAngryArnoldInfo[8];

void Player_AngryArnold_LevelHandler(Player *player) {

    AngryArnoldInfo *AAInfo = &PlayerAngryArnoldInfo[player->index];

    if (player->rings < 10) {
        AAInfo->ArnoldLevel = 0;
    } else if (player->rings < 20) {
        AAInfo->ArnoldLevel = 1;    
    } else if (player->rings < 30) {
        AAInfo->ArnoldLevel = 2;    
    } else if (player->rings < 40) {
        AAInfo->ArnoldLevel = 3;    
    } else if (player->rings < 50) {
        AAInfo->ArnoldLevel = 4;    
    } else if (player->rings < 60) {
        AAInfo->ArnoldLevel = 5;    
    } else if (player->rings < 70) {
        AAInfo->ArnoldLevel = 6;    
    } else if (player->rings < 80) {
        AAInfo->ArnoldLevel = 7;    
    } else if (player->rings < 90) {
        AAInfo->ArnoldLevel = 8;    
    } else {
        AAInfo->ArnoldLevel = 9;
    }

    switch (AAInfo->ArnoldLevel) {
        case 0:
            player->gearStats[0].boostSpeed = 200;
            break;
        case 1:
            player->gearStats[0].boostSpeed = 210;
            break;
        case 2:
            player->gearStats[0].boostSpeed = 220;
            break;
        case 3:
            player->gearStats[0].boostSpeed = 230;
            break;
        case 4:
            player->gearStats[0].boostSpeed = 240;
            break;
        case 5:
            player->gearStats[0].boostSpeed = 250;
            break;
        case 6:
            player->gearStats[0].boostSpeed = 260;
            break;
        case 7:
            player->gearStats[0].boostSpeed = 270;
            break;
        case 8:
            player->gearStats[0].boostSpeed = 280;
            break;
        case 9:
            player->gearStats[0].boostSpeed = 290;
            break;
        default:
            break;
    }
}

void Player_AngryArnold(Player *player) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);

	if (exLoads.gearExLoadID != SYBAngryArnoldEXLoad) return;
	if (player->extremeGear != PowerfulGear) return;

	AngryArnoldInfo *AAInfo = &PlayerAngryArnoldInfo[player->index];

    Player_AngryArnold_LevelHandler;

}