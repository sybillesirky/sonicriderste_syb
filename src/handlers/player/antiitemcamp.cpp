#include "cosmetics/player/exloads.hpp"
#include "gears/airtank.hpp"
#include "gears/hangon.hpp"
#include "lib/stdlib.hpp"
#include "riders/gamemode.hpp"
#include "riders/object.hpp"
#include "gears/skilllink.hpp"

ASMUsed u32 AntiItemCampHandler(Player *player, u32 item) {
	EnabledEXLoads exLoads;
	FetchEnabledEXLoadIDs(player, exLoads);
    if (CurrentGameMode != TimeTrial) {
        if (player->state == StartLine) {
            item = SpeedShoes;
        } else {
            if ((player->extremeGear == ExtremeGear::SuperHangOn && player->gearSpecificFlags[SuperHangOn::Level4]) && player->last_itemBox_random) {
                item = SpeedShoes;
            }

            if (player->extremeGear == ExtremeGear::HangOn && player->last_itemBox_random && item == SpeedShoes
                && exLoads.gearExLoadID != HangOnATEXLoad) {
                // experimental, replaces speed shoes with ten ring box instead
                item = TenRings; 
            }

            if (player->extremeGear == ExtremeGear::SkillLink) {
                SkillLinkInfo *SkLInfo = &PlayerSkillLinkInfo[player->index];

                if (item == SkLInfo->prevItem) {
                    player->speed += pSpeed(100);
                    if(!player->aiControl) PlayAudioFromDAT(Sound::ComposeSound(Sound::ID::IDKSFX, 0x3B)); //Dash panel SFX
                }
                SkLInfo->prevItem = item;
            }

            if (exLoads.gearExLoadID == TheBeastEXLoad && player->state == Run && (item == ThirtyAir || item == FiftyAir || item == HundredAir))
            {
               switch (lbl_RNG_Number(3)) {
                    case 1:
                        item = TenRings;
                        break;
                    case 2:
                        item = TwentyRings;
                        break;
                    case 3:
                        item = ThirtyRings;
                        break;
                    default:
                        item = SpeedShoes;
                        break; 
                }
            }
            
            // AirTankInfo *ATInfo = &PlayerAirTankInfo[player->index];
            // if (player->extremeGear == AirTank && ATInfo->itemUsed == 0) {
                
            //     // experimental, air tank item storage
            //     ATInfo->itemStorage[0] = item;
            //     // if (ATInfo->stoSize < 3)
            //     // {ATInfo->itemStorage[ATInfo->stoSize++] = item;} 
            //     return -1;
            // } else if (player->extremeGear == AirTank && ATInfo->itemUsed == 1) 
            // {
            //     ATInfo->itemUsed = 0;
            //     return item;
            // }
             

            if (player->last_itemID_lap != player->currentLap) {
                player->current_itemID = player->last_itemID;
            } else if (player->last_itemID == player->current_itemID) {
                item = SpeedShoes;
            } else {
                player->current_itemID = player->last_itemID;
            }
        }
    }

    player->last_itemID_lap = player->currentLap;
    return item;
}