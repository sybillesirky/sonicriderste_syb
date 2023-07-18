#include "boostarchetypenoattackretention.hpp"
#include "handlers/player/initgeardata.hpp"

void Player_BoostArchetypeNoAttackRetention(Player *player) {
    if (player->characterArchetype != BoostArchetype) return;

    if (player->input->toggleFaceButtons & XButton ||
        player->input->toggleFaceButtons & BButton) {

            // Don't set it if we're already boosting.
            if (player->MovementFlags & boosting) return;

            // Don't set it if we're not cruising.
            if (player->state != Cruise) return;
    }
}

// THIS SUCKS ASS WE CAN DO THIS AGAIN LATER FUUUUUUUCK