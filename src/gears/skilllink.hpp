#pragma once

#include "riders/player.hpp"
#include "riders/gear.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct SkillLinkInfo {
	u32 prevItem;
};
#pragma GCC diagnostic pop

extern std::array<SkillLinkInfo, MaxPlayerCount> PlayerSkillLinkInfo;

void Player_SkillLink(Player *player);
