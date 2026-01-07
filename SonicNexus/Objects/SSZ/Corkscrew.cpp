// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Corkscrew Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Corkscrew.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Corkscrew);

void Corkscrew::Update(void)
{
    foreach_active(Player, player)
    {
        if (player->state.Matches(&Player::State_Normal_Ground_Movement)) {
            if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
                if (player->groundVel > 0) {
                    if (this->direction == CORKSCREW_RIGHT && player->position.x > this->position.x)
                        player->state.Set(&Player::State_Corkscrew_Run);
                }
                else {
                    if (this->direction == CORKSCREW_LEFT && player->position.x < this->position.x)
                        player->state.Set(&Player::State_Corkscrew_Run);
                }
            }
        }
        else if (player->state.Matches(&Player::State_Rolling)) {
            if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
                if (player->groundVel > 0) {
                    if (this->direction == CORKSCREW_RIGHT && player->position.x > this->position.x)
                        player->state.Set(&Player::State_Corkscrew_Roll);
                }
                else {
                    if (this->direction == CORKSCREW_LEFT && player->position.x < this->position.x)
                        player->state.Set(&Player::State_Corkscrew_Roll);
                }
            }
        }
        else if (player->state.Matches(&Player::State_Corkscrew_Run)) {
            int32 positionDiff;
            if (player->position.x > this->position.x)
                positionDiff = ((player->position.x - this->position.x) >> 8) / 192;
            else
                positionDiff = ((player->position.x - (this->position.x - 50331648)) >> 8) / 192;

            int32 animFrame = ((positionDiff & 511) * 12) >> 9;

            if (player->animator.animationID == Player::ANI_CORKSCREW) {
                player->direction = FLIP_NONE;

                if (player->groundVel > 0) {
                    player->animator.frameID = animFrame;
                    player->rotation         = 0;
                }
                else {
                    int32 newFrame = 5 - animFrame;
                    if (newFrame < 0)
                        newFrame += 12;
                    player->animator.frameID = newFrame;
                    player->rotation         = 128;
                }
            }
            else
                player->rotation = 0;

            player->position.y = this->position.y + TO_FIXED(-32 - player->outerbox->bottom + (Math::Cos512(positionDiff) >> 4));

            if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
                if (player->groundVel > 0) {
                    if (this->direction == CORKSCREW_LEFT && player->position.x > this->position.x) {
                        player->state.Set(&Player::State_Normal_Ground_Movement);
                        player->onGround  = true;
                        player->direction = FLIP_NONE;
                    }
                }
                else {
                    if (this->direction == CORKSCREW_RIGHT && player->position.x < this->position.x) {
                        player->state.Set(&Player::State_Normal_Ground_Movement);
                        player->onGround  = true;
                        player->direction = FLIP_X;
                    }
                }
            }
        }
        else if (player->state.Matches(&Player::State_Corkscrew_Roll)) {
            int32 positionDiff;
            if (player->position.x > this->position.x)
                positionDiff = ((player->position.x - this->position.x) >> 8) / 192;
            else
                positionDiff = ((this->position.x - player->position.x) >> 8) / 192;

            player->position.y = this->position.y + TO_FIXED(-32 - player->outerbox->bottom + (Math::Cos512(positionDiff) >> 4));

            if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
                if (player->groundVel > 0) {
                    if (this->direction == CORKSCREW_LEFT) {
                        player->state.Set(&Player::State_Rolling);
                        player->onGround = true;
                    }
                }
                else {
                    if (this->direction == CORKSCREW_RIGHT) {
                        player->state.Set(&Player::State_Rolling);
                        player->onGround = true;
                    }
                }
            }
        }
    }
}

void Corkscrew::LateUpdate(void) {}
void Corkscrew::StaticUpdate(void) {}
void Corkscrew::Draw(void) {}

void Corkscrew::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
    }
}

void Corkscrew::StageLoad(void)
{
    sVars->hitbox.left   = -8;
    sVars->hitbox.top    = -64;
    sVars->hitbox.right  = 8;
    sVars->hitbox.bottom = 64;
}

#if GAME_INCLUDE_EDITOR
void Corkscrew::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Corkscrew::EditorLoad(void)
{
    sVars->aniFrames.Load("SSZ/Corkscrew.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR($(Corkscrew), direction);
    RSDK_ENUM_VAR("Right", CORKSCREW_RIGHT);
    RSDK_ENUM_VAR("Left", CORKSCREW_LEFT);
    RSDK_ENUM_VAR("Center", CORKSCREW_CENTER);
}
#endif

#if RETRO_REV0U
void Corkscrew::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Corkscrew); }
#endif

void Corkscrew::Serialize(void) { RSDK_EDITABLE_VAR(Corkscrew, VAR_ENUM, direction); }

} // namespace GameLogic
