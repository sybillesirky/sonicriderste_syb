#include "defaultgearrotation.hpp"
#include "containers/graphicalobject.hpp"
#include "lib/lib.hpp"
#include "exloads.hpp"

std::array<u16, MaxPlayerCount> PlayerDefaultIDs;

USED void InitFirstDefaultGear(Player *player) {

    u16* playerDefaultID = &PlayerDefaultIDs[player->index];

    *playerDefaultID = player->character;

    if (player->character == SuperSonic || player->character == TotalCharacterAmount) {
        *playerDefaultID = 0;
    }
    else if (player->character == Eggman) {   // This should be fine as rotating Defaults
        *playerDefaultID = 7;                   // is not possible if Eggman is selected.
    }
}

USED void RotateNextDefaultGear(GraphicalObject *object) {
    if(object->active == 0) { return; }

	const Player &player = players[object->idStruct.idIndex];
	const u8 &controllerPort = player.input->port;

    if (player.extremeGear != ExtremeGear::Default) return;
    if (player.character == Eggman) return;

    // SYB: I dunno what this does but I'm copying it for safety reasons
    #pragma clang diagnostic push
    #pragma ide diagnostic ignored "cppcoreguidelines-pro-type-reinterpret-cast"
	void **objectptr = reinterpret_cast<void **>(static_cast<u8 *>(gp2DSys) + 0x6038);
	auto *cssObject = static_cast<CSSObject *>(*objectptr);
    #pragma clang diagnostic pop

    u16* playerDefaultID = &PlayerDefaultIDs[player.index];

    // Update what ID we're on if pressing X (forward)
    if (player.input->toggleFaceButtons.hasAny(XButton) && object->idStruct.graphicalDataID == NotLockedGearSelectionID) {
        *playerDefaultID += 1;
        if (*playerDefaultID >= TotalCharacterAmount) {
            *playerDefaultID = 0;
        }
        else if (*playerDefaultID == SuperSonic || *playerDefaultID == Eggman) {
            *playerDefaultID += 1;
        }
    }

    // Update what ID we're on if pressing Y (backward)
    if (player.input->toggleFaceButtons.hasAny(YButton) && object->idStruct.graphicalDataID == NotLockedGearSelectionID) {
        if (*playerDefaultID == 0) {
            *playerDefaultID = TotalCharacterAmount - 1;
        }
        else {
            *playerDefaultID -= 1;
        }
        if (*playerDefaultID == SuperSonic || *playerDefaultID == Eggman) {
            *playerDefaultID -= 1;
        }
    }

    // Update graphics
    if(object->idStruct.graphicalDataID == NotLockedGearSelectionID) {
        object->textureIDs[0].textureSlot1 = CharacterDefaultGearData[*playerDefaultID][0]; // Portrait
        object->textureIDs[0].textureSlot5 = CharacterDefaultGearData[*playerDefaultID][1]; // GearText
    }
    else if (object->idStruct.graphicalDataID == LockedGearSelectionID) {
        object->textureIDs[0].textureSlot4 = CharacterDefaultGearData[*playerDefaultID][1]; // GearText
    }
}