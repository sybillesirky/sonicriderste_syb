#include "supersonicaura.hpp"
#include "cosmetics/player/exloads.hpp"
#include "lib/lib.hpp"
#include "riders/object.hpp"

ASMDefined void func_Particle_Task();
ASMDefined ParticleDetails lbl_001EFF98;
extern void *texList_CustomParticles;
ASMDefined ParticleDetails PerfectNazoAuraParticles;
ASMDefined ParticleDetails DarkSonicAuraParticles;
ASMDefined ParticleDetails HyperSonicAuraParticles;
ASMDefined ParticleDetails SuperKnucklesAuraParticles;

/**
 * Uses this list of matrices as the origin point for where the super aura particles are formed from.
 * See UpdateAuraMatrices() for how these matrices are formed.
 */
std::array<Matrix3x3F, MaxPlayerCount> SuperAuraOriginPoint{};

/**
 * Spawns Super Sonic aura on a specified Player.
 * The GetSuperAuraDetail() function is used to further modify what the particles look like.
 *
 * @param player The Player to spawn the particles on.
 */
ASMUsed void SuperSonicAuraCXX(Player* player) {
    if (gu32GameCnt % 15 != 0 || gu32Game_PauseFlag) return;

    SuperAuraDetail details = GetSuperAuraDetail(player);

    auto *object = static_cast<ParticleTaskObject1*>(SetTask(&func_Particle_Task, details.particles->objectGroupID, 2)->object);
    object->unk72 = 4;

    object->x = 0.0F;
    object->y = 0.0F;
    object->z = 0.0F;
    object->unk30 = 0;
    object->unk34 = 1.0F;
    object->unk38 = 0;
    object->unk3C = 0.4F;
    object->unk40 = 0.1F;
    object->unk44 = 0.1F;
    object->unk10 = 0;
    object->unk14 = 0.2F;
    object->unk18 = 0;

    object->unk48 = &gcosNnSystemVecZeroFast;
    object->unk60 = details.particles;
    object->unk68 = *details.texList;
    object->unk6C = &SuperAuraOriginPoint[player->index];
    object->unk73 = ~0U;
    object->unk74 = 0;
}

/**
 * Returns the particle specifics and the texture list it needs to use for the Super Sonic aura.
 *
 * @param player The Player to check.
 * @return The particle details.
 */
SuperAuraDetail GetSuperAuraDetail(Player* player) {
    EnabledEXLoads exLoads{};
    FetchEnabledEXLoadIDs(player, exLoads);

    SuperAuraDetail detail = {&lbl_001EFF98, &texList_CustomParticles};

    if (isSuperCharacter(*player, Knuckles)) {
        detail.particles = &SuperKnucklesAuraParticles;
    } else {
        switch (exLoads.gearExLoadID) {
            case PerfectNazo:
                detail.particles = &PerfectNazoAuraParticles;
                break;
            case DarkSonicEXLoad:
                detail.particles = &DarkSonicAuraParticles;
                break;
            case HyperSonicEXLoad:
                detail.particles = &HyperSonicAuraParticles;
                break;
			default: break;
        }
    }

    return detail;
}

/**
 * Called every frame from ASM, updates the matrices for the origin point of Super Sonic aura.
 *
 * @param player The Player who's matrices to update.
 */
ASMUsed void UpdateAuraMatrices(Player* player) {
    nnMultiplyMatrix(&player->unkC4, (gpsaMtxList_Player[player->index]) + 7, SuperAuraOriginPoint[player->index]);
    const Vector3F translation = MatrixExtractTranslation(&SuperAuraOriginPoint[player->index]);
    nnMakeTranslateMatrix(&SuperAuraOriginPoint[player->index], translation.x, translation.y, translation.z);
}