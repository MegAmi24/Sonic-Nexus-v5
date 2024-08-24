// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Parachute Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Parachute.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Parachute);

void Parachute::Update(void) { this->state.Run(this); }
void Parachute::LateUpdate(void) {}
void Parachute::StaticUpdate(void) {}

void Parachute::Draw(void)
{
    if (!this->state.Matches(&Parachute::State_Reset))
        this->animator.DrawSprite(NULL, false);
}

void Parachute::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;
        this->target        = nullptr;
        this->state.Set(&Parachute::State_Wait);
    }
}

void Parachute::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/Parachute.bin", SCOPE_GLOBAL);

    sVars->sfxCatch.Get("NexusGlobal/Catch.wav");

    sVars->hitbox.left   = -16;
    sVars->hitbox.top    = 20;
    sVars->hitbox.right  = 16;
    sVars->hitbox.bottom = 24;
}

void Parachute::State_Wait(void)
{
    SET_CURRENT_STATE();

    foreach_active(Player, player)
    {
        if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
            this->startPos = this->position;

            this->target = player;
            player->mode = Player::PLAYERMODE_PARACHUTE;
            player->animator.SetAnimation(&player->aniFrames, Player::ANI_HANGING, false, 0);
            this->position    = player->position;
            player->groundVel = player->velocity.x;
            this->position.y += TO_FIXED(player->outerbox->top - 22);
            this->state.Set(&Parachute::State_Glide);
            player->velocity.y = 0;
            player->onGround   = false;
            sVars->sfxCatch.Play();
        }
    }
}

void Parachute::State_Glide(void)
{
    SET_CURRENT_STATE();

    Player *player = this->target;

    player->animator.SetAnimation(&player->aniFrames, Player::ANI_HANGING, false, 0);

    if (player->left)
        player->direction = FLIP_X;
    if (player->right)
        player->direction = FLIP_NONE;

    if (player->direction == FLIP_NONE) {
        if (player->groundVel < -196608)
            player->groundVel = (player->groundVel * 104) >> 7;
        if (player->groundVel > 196608) {
            player->groundVel = (player->groundVel * 104) >> 7;
            if (player->groundVel < 196608)
                player->groundVel = 196608;
        }
        else {
            player->groundVel += 4096;
            if (player->groundVel > 196608)
                player->groundVel = 196608;
        }
    }
    else {
        if (player->groundVel > 196608)
            player->groundVel = (player->groundVel * 104) >> 7;
        if (player->groundVel < -196608) {
            player->groundVel = (player->groundVel * 104) >> 7;
            if (player->groundVel > -196608)
                player->groundVel = -196608;
        }
        else {
            player->groundVel -= 4096;
            if (player->groundVel < -196608)
                player->groundVel = -196608;
        }
    }

    if (player->velocity.y < 65536)
        player->velocity.y += 8192;

    player->velocity.x = player->groundVel;
    this->position     = player->position;
    this->position.y += TO_FIXED(player->outerbox->top - 22);

    if (player->state.Matches(&Player::State_Getting_Hurt)) {
        this->active = ACTIVE_NORMAL;
        this->state.Set(&Parachute::State_Fall);
        player->mode     = Player::PLAYERMODE_NORMAL;
        this->velocity.x = player->velocity.x;
    }

    if (player->onGround) {
        this->active = ACTIVE_NORMAL;
        this->state.Set(&Parachute::State_Fall);
        player->mode     = Player::PLAYERMODE_NORMAL;
        this->velocity.x = player->velocity.x;
    }
    else
        this->velocity.y = player->velocity.y;

    if (player->jumpPress) {
        Player::sVars->sfxJump.Play();
        player->state.Set(&Player::State_Air_Movement);
        player->animator.SetAnimation(&player->aniFrames, Player::ANI_JUMPING, false, 0);
        player->velocity.y = -262144;
        player->onGround   = false;

        this->active = ACTIVE_NORMAL;
        this->state.Set(&Parachute::State_Fall);
        player->mode     = Player::PLAYERMODE_NORMAL;
        this->velocity.x = player->velocity.x;
    }

    this->animator.Process();
}

void Parachute::State_Fall(void)
{
    SET_CURRENT_STATE();

    if (this->velocity.y < 65536)
        this->velocity.y += 8192;

    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;

    if (!this->CheckOnScreen(NULL)) {
        this->state.Set(&Parachute::State_Reset);
        this->position = this->startPos;
    }

    this->animator.Process();
}

void Parachute::State_Reset(void)
{
    SET_CURRENT_STATE();

    if (!this->CheckOnScreen(NULL)) {
        this->state.Set(&Parachute::State_Wait);
        this->active           = ACTIVE_BOUNDS;
        this->animator.frameID = 0;
    }
}

#if GAME_INCLUDE_EDITOR
void Parachute::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Parachute::EditorLoad(void) { sVars->aniFrames.Load("SSZ/Parachute.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void Parachute::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Parachute); }
#endif

void Parachute::Serialize(void) {}

} // namespace GameLogic
