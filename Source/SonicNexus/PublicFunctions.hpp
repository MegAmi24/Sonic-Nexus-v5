#pragma once
#include "All.hpp"

using namespace RSDK;

#if RETRO_USE_MOD_LOADER
namespace RSDK
{
namespace Mod
{

template <typename NS, typename Type> inline static void AddPublicFunction(const char *functionName, Type(NS::*functionPtr))
{
    modTable->AddPublicFunction(functionName, reinterpret_cast<void *&>(functionPtr));
}

} // namespace Mod
} // namespace RSDK

#define ADD_PUBLIC_FUNC(func) RSDK::Mod::AddPublicFunction(#func, &func)

namespace GameLogic
{

static void InitPublicFunctions()
{
    // NexusGlobals/ActFinish
    ADD_PUBLIC_FUNC(ActFinish::State_MoveIn);
    ADD_PUBLIC_FUNC(ActFinish::State_Wait);
    ADD_PUBLIC_FUNC(ActFinish::State_AddEverying);
    ADD_PUBLIC_FUNC(ActFinish::State_WaitSomeMore);
    ADD_PUBLIC_FUNC(ActFinish::State_FadeOut);

    // NexusGlobals/Camera
    ADD_PUBLIC_FUNC(Camera::SetPlayerScreenPosition);
    ADD_PUBLIC_FUNC(Camera::SetPlayerScreenPositionCDStyle);
    ADD_PUBLIC_FUNC(Camera::SetPlayerLockedScreenPosition);

    // NexusGlobals/DeathEvent
    ADD_PUBLIC_FUNC(DeathEvent::State_GameOver);
    ADD_PUBLIC_FUNC(DeathEvent::State_FadeToBlack);

    // NexusGlobals/HUD
    ADD_PUBLIC_FUNC(HUD::State_Normal);
    ADD_PUBLIC_FUNC(HUD::State_Enter);
    ADD_PUBLIC_FUNC(HUD::DrawNumbers);

    // NexusGlobals/Invincibility
    ADD_PUBLIC_FUNC(Invincibility::State_Spawner);
    ADD_PUBLIC_FUNC(Invincibility::State_Child);

    // NexusGlobals/Monitor
    ADD_PUBLIC_FUNC(Monitor::State_Idle);
    ADD_PUBLIC_FUNC(Monitor::State_Powerup_Rise);
    ADD_PUBLIC_FUNC(Monitor::State_Powerup_Show);
    ADD_PUBLIC_FUNC(Monitor::State_Broken);

    // NexusGlobals/Music
    ADD_PUBLIC_FUNC(Music::SetTrack);
    ADD_PUBLIC_FUNC(Music::Play);
    ADD_PUBLIC_FUNC(Music::Stop);
    ADD_PUBLIC_FUNC(Music::Pause);
    ADD_PUBLIC_FUNC(Music::Resume);
    ADD_PUBLIC_FUNC(Music::CurrentTrack);
    ADD_PUBLIC_FUNC(Music::SetVolume);

    // NexusGlobals/Ring
    ADD_PUBLIC_FUNC(Ring::State_Normal);
    ADD_PUBLIC_FUNC(Ring::State_Lose);
    ADD_PUBLIC_FUNC(Ring::State_Sparkle);

    // NexusGlobals/SignPost
    ADD_PUBLIC_FUNC(SignPost::State_Waiting);
    ADD_PUBLIC_FUNC(SignPost::State_Spinning);
    ADD_PUBLIC_FUNC(SignPost::State_Exit);

    // NexusGlobals/StarPost
    ADD_PUBLIC_FUNC(StarPost::State_Normal);
    ADD_PUBLIC_FUNC(StarPost::State_Swinging);
    ADD_PUBLIC_FUNC(StarPost::State_Flashing);
    ADD_PUBLIC_FUNC(StarPost::Draw_Swinging);
    ADD_PUBLIC_FUNC(StarPost::Draw_Flashing);

    // NexusGlobals/TitleCard
    ADD_PUBLIC_FUNC(TitleCard::State_FadeUp);
    ADD_PUBLIC_FUNC(TitleCard::State_SideBar);
    ADD_PUBLIC_FUNC(TitleCard::State_Word);
    ADD_PUBLIC_FUNC(TitleCard::State_Zone);
    ADD_PUBLIC_FUNC(TitleCard::State_ActBall);
    ADD_PUBLIC_FUNC(TitleCard::State_VertText_Down);
    ADD_PUBLIC_FUNC(TitleCard::State_VertText_Up);
    ADD_PUBLIC_FUNC(TitleCard::State_ColourCircle);
    ADD_PUBLIC_FUNC(TitleCard::State_SideBar_Exit);
    ADD_PUBLIC_FUNC(TitleCard::State_FirstWord_Exit);
    ADD_PUBLIC_FUNC(TitleCard::State_SecondWord_Exit);
    ADD_PUBLIC_FUNC(TitleCard::State_Zone_Exit);
    ADD_PUBLIC_FUNC(TitleCard::State_ActBall_Exit);
    ADD_PUBLIC_FUNC(TitleCard::Draw_GreyscaleBG);
    ADD_PUBLIC_FUNC(TitleCard::Draw_ColourCircle);
    ADD_PUBLIC_FUNC(TitleCard::Draw_Word);
    ADD_PUBLIC_FUNC(TitleCard::Draw_VertText);

    // NexusIntro/Intro
    ADD_PUBLIC_FUNC(Intro::State_LoadTheVideo);
    ADD_PUBLIC_FUNC(Intro::State_FadeToWhite);
    ADD_PUBLIC_FUNC(Intro::VideoSkipCB);

    // NexusIntro/RLogo
    ADD_PUBLIC_FUNC(RLogo::State_FadeFromBlack);
    ADD_PUBLIC_FUNC(RLogo::State_Wait);
    ADD_PUBLIC_FUNC(RLogo::State_FadeToBlack);

    // NexusTitle/Logo
    ADD_PUBLIC_FUNC(Logo::State_BouncingDown);
    ADD_PUBLIC_FUNC(Logo::State_BouncingScale);
    ADD_PUBLIC_FUNC(Logo::State_Static);
    ADD_PUBLIC_FUNC(Logo::Draw_Normal);
    ADD_PUBLIC_FUNC(Logo::Draw_BouncingScale);

    // NexusTitle/PressStart
    ADD_PUBLIC_FUNC(PressStart::State_AwaitStart);
    ADD_PUBLIC_FUNC(PressStart::State_FadeToBlack);

    // Players/Player
    ADD_PUBLIC_FUNC(Player::ProcessPlayerControl);
    ADD_PUBLIC_FUNC(Player::SetMovementStats);
    ADD_PUBLIC_FUNC(Player::ProcessDefaultAirMovement);
    ADD_PUBLIC_FUNC(Player::ProcessDefaultGravityFalse);
    ADD_PUBLIC_FUNC(Player::ProcessDefaultGravityTrue);
    ADD_PUBLIC_FUNC(Player::ProcessDefaultGroundMovement);
    ADD_PUBLIC_FUNC(Player::ProcessDefaultJumpAction);
    ADD_PUBLIC_FUNC(Player::ProcessDefaultRollingMovement);
    ADD_PUBLIC_FUNC(Player::ProcessDebugMode);
    ADD_PUBLIC_FUNC(Player::ProcessPlayerAnimation);
    ADD_PUBLIC_FUNC(Player::Main);
    ADD_PUBLIC_FUNC(Player::State_Normal_Ground_Movement);
    ADD_PUBLIC_FUNC(Player::State_Air_Movement);
    ADD_PUBLIC_FUNC(Player::State_Rolling);
    ADD_PUBLIC_FUNC(Player::State_Rolling_Jump);
    ADD_PUBLIC_FUNC(Player::State_Looking_Up);
    ADD_PUBLIC_FUNC(Player::State_Looking_Down);
    ADD_PUBLIC_FUNC(Player::State_Spindash);
    ADD_PUBLIC_FUNC(Player::State_Peelout);
    ADD_PUBLIC_FUNC(Player::State_Getting_Hurt);
    ADD_PUBLIC_FUNC(Player::State_Hurt_Recoil);
    ADD_PUBLIC_FUNC(Player::State_Dying);
    ADD_PUBLIC_FUNC(Player::State_Drowning);
    ADD_PUBLIC_FUNC(Player::State_Hanging);
    ADD_PUBLIC_FUNC(Player::State_Corkscrew_Run);
    ADD_PUBLIC_FUNC(Player::State_Corkscrew_Roll);
    ADD_PUBLIC_FUNC(Player::State_Tube_Rolling);
    ADD_PUBLIC_FUNC(Player::HandleMovement);
    ADD_PUBLIC_FUNC(Player::BoxCollision);
    ADD_PUBLIC_FUNC(Player::PlatformCollision);

    // SSZ/Chompy
    ADD_PUBLIC_FUNC(Chompy::State_Wait);
    ADD_PUBLIC_FUNC(Chompy::State_Jump);

    // SSZ/CLedge
    ADD_PUBLIC_FUNC(CLedge::State_Ledge);
    ADD_PUBLIC_FUNC(CLedge::State_Wait_Right);
    ADD_PUBLIC_FUNC(CLedge::State_Wait_Left);
    ADD_PUBLIC_FUNC(CLedge::State_Crumble_Left);
    ADD_PUBLIC_FUNC(CLedge::State_Crumble_Right);
    ADD_PUBLIC_FUNC(CLedge::State_Wait_Respawn);
    ADD_PUBLIC_FUNC(CLedge::State_FallingBlock);
    ADD_PUBLIC_FUNC(CLedge::Draw_Ledge);
    ADD_PUBLIC_FUNC(CLedge::Draw_Crumble_Left);
    ADD_PUBLIC_FUNC(CLedge::Draw_FallingBlock);

    // SSZ/HelliBomber
    ADD_PUBLIC_FUNC(HelliBomber::State_Idle);
    ADD_PUBLIC_FUNC(HelliBomber::State_Move);
    ADD_PUBLIC_FUNC(HelliBomber::State_Wait);
    ADD_PUBLIC_FUNC(HelliBomber::State_Shoot);
    ADD_PUBLIC_FUNC(HelliBomber::State_Bullet);

    // SSZ/MRally
    ADD_PUBLIC_FUNC(MRally::State_Move_Left);
    ADD_PUBLIC_FUNC(MRally::State_Move_Right);
    ADD_PUBLIC_FUNC(MRally::State_Move_Left_Fast);
    ADD_PUBLIC_FUNC(MRally::State_Move_Right_Fast);
    ADD_PUBLIC_FUNC(MRally::State_Wait_Left);
    ADD_PUBLIC_FUNC(MRally::State_Wait_Right);

    // SSZ/Parachute
    ADD_PUBLIC_FUNC(Parachute::State_Wait);
    ADD_PUBLIC_FUNC(Parachute::State_Glide);
    ADD_PUBLIC_FUNC(Parachute::State_Fall);
    ADD_PUBLIC_FUNC(Parachute::State_Reset);

    // SSZ/ZipLine
    ADD_PUBLIC_FUNC(ZipLine::State_Stopped);
    ADD_PUBLIC_FUNC(ZipLine::State_Moving);
    ADD_PUBLIC_FUNC(ZipLine::State_Return);

} // static void InitPublicFunctions

} // namespace GameLogic

#endif