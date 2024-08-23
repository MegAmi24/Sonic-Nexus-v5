// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: TubeSwitch Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "TubeSwitch.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(TubeSwitch);

void TubeSwitch::Update(void)
{
    foreach_active(Player, player)
    {
        if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
            switch (this->type) {
                case TUBESWITCH_R_ENTRY:
                    if (player->groundVel > 0) {
                        if (!player->state.Matches(&Player::State_Tube_Rolling))
                            Player::sVars->sfxSpin.Play();
                        player->state.Set(&Player::State_Tube_Rolling);
                        player->animator.SetAnimation(player->aniFrames, Player::ANI_JUMPING, false, 0);
                        player->minRollSpeed = 786432;
                    }
                    else {
                        player->state.Set(&Player::State_Rolling);
                        player->animator.SetAnimation(player->aniFrames, Player::ANI_JUMPING, false, 0);
                    }
                    break;
                case TUBESWITCH_L_ENTRY:
                    if (player->groundVel < 0) {
                        if (!player->state.Matches(&Player::State_Tube_Rolling))
                            Player::sVars->sfxSpin.Play();
                        player->state.Set(&Player::State_Tube_Rolling);
                        player->animator.SetAnimation(player->aniFrames, Player::ANI_JUMPING, false, 0);
                        player->minRollSpeed = 786432;
                    }
                    else {
                        player->state.Set(&Player::State_Rolling);
                        player->animator.SetAnimation(player->aniFrames, Player::ANI_JUMPING, false, 0);
                    }
                    break;
                case TUBESWITCH_BOOST:
                    if (!player->state.Matches(&Player::State_Tube_Rolling))
                        Player::sVars->sfxSpin.Play();
                    player->state.Set(&Player::State_Tube_Rolling);
                    player->animator.SetAnimation(player->aniFrames, Player::ANI_JUMPING, false, 0);
                    player->minRollSpeed  = 786432;
                    player->collisionMode = CMODE_LWALL;
                    player->onGround      = true;
                    player->groundVel     = 786432;
                    break;
                case TUBESWITCH_ROLL:
                    player->state.Set(&Player::State_Rolling);
                    player->animator.SetAnimation(player->aniFrames, Player::ANI_JUMPING, false, 0);
                    break;
                case TUBESWITCH_EXIT: player->collisionPlane = 0; break;
                case TUBESWITCH_ENTRY:
                    if (!player->state.Matches(&Player::State_Tube_Rolling))
                        Player::sVars->sfxSpin.Play();
                    player->state.Set(&Player::State_Tube_Rolling);
                    player->animator.SetAnimation(player->aniFrames, Player::ANI_JUMPING, false, 0);
                    player->minRollSpeed = 786432;
                    break;
            }
        }
    }
}

void TubeSwitch::LateUpdate(void) {}
void TubeSwitch::StaticUpdate(void) {}
void TubeSwitch::Draw(void) {}

void TubeSwitch::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        if (data)
            this->type = (TubeSwitchTypes)(VOID_TO_INT(data));
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
    }
}

void TubeSwitch::StageLoad(void)
{
    sVars->hitbox.left   = -16;
    sVars->hitbox.top    = -16;
    sVars->hitbox.right  = 16;
    sVars->hitbox.bottom = 16;
}

#if GAME_INCLUDE_EDITOR
void TubeSwitch::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 5);
    this->animator.DrawSprite(NULL, false);
}

void TubeSwitch::EditorLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Editor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TubeSwitch::sVars, type);
    RSDK_ENUM_VAR("Right Entry", TUBESWITCH_R_ENTRY);
    RSDK_ENUM_VAR("Left Entry", TUBESWITCH_L_ENTRY);
    RSDK_ENUM_VAR("Boost", TUBESWITCH_BOOST);
    RSDK_ENUM_VAR("Roll", TUBESWITCH_ROLL);
    RSDK_ENUM_VAR("Exit", TUBESWITCH_EXIT);
    RSDK_ENUM_VAR("Entry", TUBESWITCH_ENTRY);
}
#endif

#if RETRO_REV0U
void TubeSwitch::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(TubeSwitch); }
#endif

void TubeSwitch::Serialize(void) { RSDK_EDITABLE_VAR(TubeSwitch, VAR_ENUM, type); }

} // namespace GameLogic
