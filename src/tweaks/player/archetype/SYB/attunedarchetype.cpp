#include "attunedarchetype.hpp"
#include "handlers/player/initgeardata.hpp"

void Player_AttunedArchetype(Player *player) {
    if (player->characterArchetype != AttunedArchetype) return;

    if (player->state == RailGrind) {
        player->speed = pSpeed(900);
    }
}