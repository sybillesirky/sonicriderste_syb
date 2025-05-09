#include "mechanics/magneticimpulse.hpp" // C24119A0
#include "handlers/player/deathplane.hpp" // C240F4F8
#include "mechanics/slipstream.hpp" // C240F4C4
#include "mechanics/fastfall.hpp" // C240F4EC
#include "cosmetics/player/hypersonic_lighting.hpp"
#include "handlers/player/timerhandler.hpp" // C240C9EC C2415B74 C240F4DC C24119A8 C24119CC
#include "gears/beginner.hpp" // C24119B0
#include "gears/coverf.hpp" // C24119D4
#include "gears/fastest.hpp" // C2411994
#include "gears/supertails.hpp" // C24119AC
#include "gears/supermetal.hpp"
#include "mechanics/dash/grindraildash.hpp"
#include "mechanics/dash/flyhoopdash.hpp"
#include "mechanics/lcanceling.hpp"
#include "mechanics/characters/gizoidreplication.hpp"
#include "gears/thecrazy.hpp"
#include "gears/hangon.hpp"
#include "gears/advantagef.hpp"
#include "gears/magiccarpet.hpp"
#include "gears/covers.hpp"
#include "gears/coverp.hpp"
#include "gears/powergear.hpp"
#include "gears/tweaks/ringgearlevels.hpp"
#include "gears/stardustspeederII.hpp"
#include "tweaks/player/archetype/boostarchetypejcbc.hpp"
#include "gears/speedbalancer.hpp"
#include "handlers/customcodehandler.hpp"
#include "gears/slidebooster.hpp"
#include "handlers/player/zbutton.hpp"
#include "mechanics/airoutbutton.hpp"
#include "gears/turbostar.hpp"
#include "gears/faster.hpp"
#include "mechanics/autopilot.hpp"
#include "gears/hypersonic.hpp"
#include "mechanics/archetype/afterburner.hpp"
#include "handlers/player/specialflagtweaks.hpp"
#include "gears/blastGaugeGears.hpp"
//#include "gears/faster.hpp"
//#include "gears/hyperhangon.hpp"
//#include "gears/slidebooster.hpp"
//#include "gears/olliekinggear.hpp"
//#include "gears/dynamo.hpp"
//#include "gears/turbostar.hpp"
// #include "gears/airtank.hpp"
//#include "gears/eggsterminator.hpp"
#include "handlers/menu/debugmenu/debugmenu_handlers.hpp"
#include "mechanics/tricklevelling.hpp"
#include "gears/windstar.hpp"
#include "gears/airship.hpp"
#include "gears/gshot.hpp"
#include "gears/wanted.hpp"
#include "gears/archangel.hpp"
#include "gears/reservetank.hpp"
#include "gears/warpdrive.hpp"
#include "gears/challenger.hpp"
#include "gears/projektred.hpp"
#include "gears/virtuoso.hpp"
#include "gears/greed.hpp"
#include "gears/skilllink.hpp"

// ASMDefined void Player_Autopilot(Player *player); // _Main/text.s ; C240F4D8

ASMUsed void PlayerHandler(Player *player) {
    // this is just a main function that calls all the handlers

    if (player->state != Fly) player->splashCanyonFlyRoute = FALSE;

    lbl_Slipstream(player);
    lbl_FastFall(player);
    HyperSonic_RainbowLighting(player);
    Player_DeathPlane(player);
    Player_TimerHandler(*player);
    Player_Autopilot(player);
    Player_MagneticImpulseTimer(*player);
    Player_SuperTailsTransformation(*player);
    Player_SuperMetalTransformation(*player);
    Player_CheckBeginnerAirOut(player);
    Player_GrindRailDashHandler(player);
    Player_FlyHoopDashHandler(player);
    Player_LCancelHandler(player);
    Player_GRHandler(player);
    Player_DriftTips(player);
    Player_LimiterCut(player);
    Player_RingGearLevels(player);
    Player_BoostArchetypeJCBC(player);
    Player_SpeedBalancerResetBoostSpeed(player);
    CustomCodehandler_Player(player);
    Player_HyperdriveHandler(player);
    Player_HyperSonicUpdatePlayerStats(player);
    Player_HyperSonicTotalLinkCalc(player);
    Player_HyperSonicRingStream(player);
    // Player_HyperHangOn(player);
    // Player_TheProfessional(player);
    // Player_OllieKingGear(player);
    // Player_Dynamo(player);
    // Player_AirTank(player);
    // Player_Eggsterminator(player);
    // Player_TheBeast(player);
    Player_IgnoreTurbulenceHandler(player);
    Player_AirOutButton(player);
    Player_Afterburner(player);
    Player_windcatcher(player);
    // Player_ZGSuperSonic(player);
    // Player_Excalibur(player);
    // Player_HangOnAT(player);
    // Player_NeoII(player);
    // Player_SuperStorm(player);
    Player_lightBoardEffect(player);
    Player_storeFlagInfo(player);
    Player_checkGearIfUsesBlastGauge(player);
    Player_resetGauge(player);
    Player_TornadoBoostApplications(player);
    DebugMenuHandler_InfiniteAir(player);
    DebugMenuHandler_InfiniteRings(player);

    switch(player->extremeGear) {
        using namespace ExtremeGear;
        case ExtremeGear::TurboStar:
            Player_TurboStar(player);
            break;
        case PowerGear:
            Player_PowerGear(player);
            break;
        case Fastest:
            Player_Fastest(player);
            break;
        case Faster:
            Player_Faster(player);
            break;
        case Beginner:
            Player_BeginnerOmegaBoost(player);
            break;
        case SlideBooster:
            Player_SlideBooster(player);
            break;
        case TheCrazy:
            Player_CrazyStop(player);
            break;
        case MagicCarpet:
        	Player_MagicCarpet(player);
            break;
        case CoverS:
            Player_CoverS(player);
            break;
        case CoverF:
            Player_CoverF(player);
            break;
        case CoverP:
            Player_CoverP(player);
            Player_moneyCrisis(player);
            break;
        case ExtremeGear::HangOn:
            Player_HangOn(player);
            break;
        case AdvantageF:
            Player_AdvantageF(player);
            break;
        case GunGear:
            Player_StardustSpeederII(player);
            Player_SSIIRingBoost(player);
            break;
        case ShootingStar:
            Player_TrickLevelling(player);
            break;
        case WindStar:
            Player_TrickLevelling(player);
            Player_WindStar(player);
            break;
        case RoadStar:
            Player_TrickLevelling(player);
            break;
        case Airship:
            Player_Airship(player);
            break;
        case GShot:
            Player_GShot(player);
            break;
        case ExtremeGear::Wanted:
            Player_Wanted(player);
            break;
        case Archangel:
            Player_ArchAngel(player);
            break;
        case ExtremeGear::ReserveTank:
            Player_ReserveTank(player);
            break;
        case WarpDrive:
            Player_WarpDrive(player);
            break;
        case ExtremeGear::Challenger:
            Player_Challenger(player);
            break;
        case ExtremeGear::ProjektRed:
            Player_ProjektRed(player);
            break;
        case ExtremeGear::Virtuoso:
            Player_Virtuoso(player);
            break;
        case ExtremeGear::Greed:
            Player_Greed(player);
            break;
        case ExtremeGear::SkillLink:
            Player_SkillLink(player);
            break;
        default:
            break;
    }
}