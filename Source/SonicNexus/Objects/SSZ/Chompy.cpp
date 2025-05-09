// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Chompy Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Chompy.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Chompy);

void Chompy::Update(void)
{
    this->state.Run(this);

    this->animator.Process();

    foreach_active(Player, player)
    {
        if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
            switch (player->animator.animationID) {
                case Player::ANI_SPINDASH:
                case Player::ANI_JUMPING:
                    CREATE_ENTITY(Explosion, NULL, this->position.x, this->position.y);
                    this->Destroy();
                    $(Player)->sfxDestroy.Play();
                    if (player->velocity.y > 0)
                        player->velocity.y = -player->velocity.y;
                    globals->score += 100;
                    break;
                default:
                    if (RSDK_GET_ENTITY_GEN(SLOT_POWERUP1)->classID == $(Invincibility)->classID) {
                        CREATE_ENTITY(Explosion, NULL, this->position.x, this->position.y);
                        this->Destroy();
                        $(Player)->sfxDestroy.Play();
                        if (player->velocity.y > 0)
                            player->velocity.y = -player->velocity.y;
                        globals->score += 100;
                    }
                    else if (!player->invincibility) {
                        player->state.Set(&Player::State_Getting_Hurt);
                        if (player->position.x > this->position.x)
                            player->groundVel = 131072;
                        else
                            player->groundVel = -131072;
                    }
                    break;
            }
        }
    }
}

void Chompy::LateUpdate(void) {}
void Chompy::StaticUpdate(void) {}
void Chompy::Draw(void) { this->animator.DrawSprite(NULL, false); }

void Chompy::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->jumpDir = (ChompyJumpDirs)(VOID_TO_INT(data));
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;
        this->drawFX        = FX_ROTATE;
        this->state.Set(&Chompy::State_Wait);
    }
}

void Chompy::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/Chompy.bin", SCOPE_GLOBAL);

    sVars->hitbox.left   = -14;
    sVars->hitbox.top    = -14;
    sVars->hitbox.right  = 14;
    sVars->hitbox.bottom = 14;
}

void Chompy::State_Wait(void)
{
    SET_CURRENT_STATE();

    if (++this->timer == 1) {
        this->timer    = 0;
        this->oldPos.y = this->position.y;
        if (this->jumpDir == JUMPDIR_LEFT) {
            this->velocity.x = -131072;
            this->rotation   = 512;
        }
        else {
            this->velocity.x = 131072;
            this->rotation   = 0;
        }
        this->velocity.y = -638976;
        this->state.Set(&Chompy::State_Jump);
        this->active = ACTIVE_NORMAL;
    }
}

void Chompy::State_Jump(void)
{
    SET_CURRENT_STATE();

    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;

    this->velocity.y += 12288;

    if (this->jumpDir == JUMPDIR_LEFT)
        this->rotation -= 3;
    else
        this->rotation += 3;

    if (this->velocity.y > 638976) {
        this->active = ACTIVE_BOUNDS;
        this->state.Set(&Chompy::State_Wait);
        this->position.y = this->oldPos.y;
        this->jumpDir    = this->jumpDir == JUMPDIR_LEFT ? JUMPDIR_RIGHT2 : JUMPDIR_LEFT;
    }
}

#if GAME_INCLUDE_EDITOR
void Chompy::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Chompy::EditorLoad(void)
{
    sVars->aniFrames.Load("SSZ/Chompy.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR($(Chompy), jumpDir);
    RSDK_ENUM_VAR("Left", JUMPDIR_LEFT);
    RSDK_ENUM_VAR("Right", JUMPDIR_RIGHT);
}
#endif

#if RETRO_REV0U
void Chompy::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Chompy); }
#endif

void Chompy::Serialize(void) { RSDK_EDITABLE_VAR(Chompy, VAR_ENUM, jumpDir); }

} // namespace GameLogic
