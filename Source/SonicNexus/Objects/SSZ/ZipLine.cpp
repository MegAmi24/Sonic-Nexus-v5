// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: ZipLine Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "ZipLine.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(ZipLine);

void ZipLine::Update(void)
{
    this->lastY = this->position.y;

    this->state.Run(this);

    this->angle = 256 - ((this->trackPos >> 8) / 404);

    int32 angle = this->angle;
    if (angle > 256) {
        angle       = 256;
        this->angle = ((this->angle - 256) << 3) + 256;
    }
    if (angle < 0) {
        angle       = 0;
        this->angle = (this->angle << 3) + 512;
    }

    this->position.x = cos512(angle) * 25856 + this->originPos.x;
    this->position.y = sin512(angle) * 53760 + this->originPos.y;

    this->handlePos.x = cos512(this->angle) * 4608;
    this->handlePos.x += this->position.x;
    this->handlePos.x &= 4294901760;

    this->handlePos.y = sin512(this->angle) * 4608;
    this->handlePos.y += this->position.y;
    this->handlePos.y &= 4294901760;

    foreach_active(Player, player)
    {
        RSDK::Vector2 posStore = this->position;
        this->position         = this->handlePos;

        if (player->state.Matches(&Player::State_Hanging)) {
            if (this->target == player) {
                player->position.x = this->position.x;
                player->position.y = this->position.y - TO_FIXED(player->outerbox->top) + 786432;

                if (player->jumpPress) {
                    player->angle     = 0;
                    player->groundVel = this->groundVel;
                    Player::DefaultJumpAction(player);
                    player->state.Set(&Player::State_Air_Movement);
                    Player::sVars->sfxJump.Play();
                    this->state.Set(&ZipLine::State_Return);
                    this->target = nullptr;
                    player->velocity.y += (posStore.y - this->lastY) >> 1;
                }
            }
        }
        else if (this->state.Matches(&ZipLine::State_Stopped) && this->trackPos > -1) {
            if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
                player->position.x = this->position.x;
                player->position.y = this->position.y - TO_FIXED(player->outerbox->top) + 786432;
                player->state.Set(&Player::State_Hanging);
                player->animator.SetAnimation(player->aniFrames, Player::ANI_HANGING, false, 0);
                this->state.Set(&ZipLine::State_Moving);
                this->active = ACTIVE_NORMAL;
                this->target = player;

                if (player->velocity.y > 0) {
                    this->groundVel = 81920;
                    if (posStore.x < this->originPos.x)
                        this->groundVel += player->velocity.y >> 3;
                    else
                        this->groundVel -= player->velocity.y >> 3;
                }
                else
                    this->groundVel = 81920;

                sVars->sfxCatch.Play();
            }
        }

        this->position = posStore;
    }
}

void ZipLine::LateUpdate(void) {}
void ZipLine::StaticUpdate(void) {}

void ZipLine::Draw(void)
{
    this->animator.frameID = 0;
    this->animator.DrawSprite(NULL, false);

    int32 height = 8;
    for (int32 s = 0; s < 3; s++) {
        RSDK::Vector2 pos;
        pos.x = (cos512(this->angle) * height) << 7;
        pos.x += this->position.x;

        pos.y = (sin512(this->angle) * height) << 7;
        pos.y += this->position.y;

        this->animator.DrawSprite(&pos, false);
        height += 8;
    }

    this->animator.frameID = 1;
    this->animator.DrawSprite(&this->handlePos, false);
}

void ZipLine::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;

        this->state.Set(&ZipLine::State_Stopped);
        this->originPos = this->position;
        this->originPos.x += 13238272;
        this->target = nullptr;
    }
}

void ZipLine::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/ZipLine.bin", SCOPE_GLOBAL);

    sVars->sfxCatch.Get("NexusGlobal/Catch.wav");

    sVars->hitbox.left   = -8;
    sVars->hitbox.top    = 2;
    sVars->hitbox.right  = 8;
    sVars->hitbox.bottom = 16;
}

void ZipLine::State_Stopped(void)
{
    SET_CURRENT_STATE();

    this->groundVel += 2560;
    this->trackPos += this->groundVel;
    if (this->trackPos > 0) {
        this->trackPos  = 0;
        this->groundVel = 0;
    }
}

void ZipLine::State_Moving(void)
{
    SET_CURRENT_STATE();

    if (this->position.x < this->originPos.x)
        this->groundVel += 5120;
    else
        this->groundVel -= 5120;
    this->trackPos += this->groundVel;

    if (this->target && !this->target->state.Matches(&Player::State_Hanging)) {
        this->state.Set(&ZipLine::State_Return);
        this->target = nullptr;
    }
}

void ZipLine::State_Return(void)
{
    SET_CURRENT_STATE();

    if (this->position.x < this->originPos.x)
        this->groundVel += 2560;
    else
        this->groundVel -= 2560;
    this->trackPos += this->groundVel;

    if (this->trackPos < 1) {
        this->state.Set(&ZipLine::State_Stopped);
        if (this->groundVel < TO_FIXED(-1))
            this->groundVel = TO_FIXED(-1);
    }
}

#if GAME_INCLUDE_EDITOR
void ZipLine::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void ZipLine::EditorLoad(void) { sVars->aniFrames.Load("SSZ/ZipLine.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void ZipLine::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(ZipLine); }
#endif

void ZipLine::Serialize(void) {}

} // namespace GameLogic
