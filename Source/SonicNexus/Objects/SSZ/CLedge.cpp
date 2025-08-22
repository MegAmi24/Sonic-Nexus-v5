// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: CLedge Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "CLedge.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(CLedge);

void CLedge::Update(void)
{
    this->state.Run(this);

    RSDK::Hitbox hitbox;
    hitbox.left   = this->left;
    hitbox.top    = -16;
    hitbox.right  = this->right;
    hitbox.bottom = 16;

    foreach_active(Player, player)
    {
        if (this->state.Matches(&CLedge::State_Ledge)) {
            if (player->PlatformCollision(this, &hitbox)) {
                this->timer  = 15;
                this->active = ACTIVE_NORMAL;
                if (player->position.x > this->position.x)
                    this->state.Set(&CLedge::State_Wait_Right);
                else
                    this->state.Set(&CLedge::State_Wait_Left);
            }
        }
        else
            player->PlatformCollision(this, &hitbox);
    }
}

void CLedge::LateUpdate(void) {}
void CLedge::StaticUpdate(void) {}
void CLedge::Draw(void) { this->stateDraw.Run(this); }

void CLedge::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->length = VOID_TO_INT(data);
        this->animator.SetAnimation(sVars->aniFrames, 3, true, 0);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(128);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;

        if (this->length) {
            this->state.Set(&CLedge::State_Ledge);
            this->stateDraw.Set(&CLedge::Draw_Ledge);
            this->noBlocks = this->length;
            this->right    = this->length << 3;
            this->left     = -this->right;
        }
        else {
            this->state.Set(&CLedge::State_FallingBlock);
            this->stateDraw.Set(&CLedge::Draw_FallingBlock);
        }
    }
}

void CLedge::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/Platform.bin", SCOPE_GLOBAL);
    sVars->sfxCrumble.Get("SSZ/Crumble.wav");
}

void CLedge::State_Ledge(void)
{
    SET_CURRENT_STATE();

    // Nothing happens here, see the foreach Player code in Update()
}

void CLedge::State_Wait_Right(void)
{
    SET_CURRENT_STATE();

    if (this->timer)
        this->timer--;
    else {
        sVars->sfxCrumble.Play();
        this->state.Set(&CLedge::State_Crumble_Right);
    }
}

void CLedge::State_Wait_Left(void)
{
    SET_CURRENT_STATE();

    if (this->timer)
        this->timer--;
    else {
        sVars->sfxCrumble.Play();
        this->state.Set(&CLedge::State_Crumble_Left);
        this->stateDraw.Set(&CLedge::Draw_Crumble_Left);
    }
}

void CLedge::State_Crumble_Left(void)
{
    SET_CURRENT_STATE();

    if (++this->timer == 6) {
        this->timer = 0;
        if (this->left < this->right) {
            this->noBlocks--;
            this->left += 16;

            RSDK::Vector2 blockPos;
            blockPos.x = TO_FIXED(this->left - 8) + this->position.x;
            blockPos.y = this->position.y - 655360;

            CLedge *block           = CREATE_ENTITY(CLedge, NULL, blockPos.x, blockPos.y);
            block->velocity.y       = -65536;
            block->animator.frameID = 1;

            blockPos.y += 1310720;
            block                   = CREATE_ENTITY(CLedge, NULL, blockPos.x, blockPos.y);
            block->animator.frameID = 2;
        }
        else
            this->Destroy();
    }
}

void CLedge::State_Crumble_Right(void)
{
    SET_CURRENT_STATE();

    if (++this->timer == 6) {
        this->timer = 0;
        if (this->right > this->left) {
            this->noBlocks--;
            this->right -= 16;

            RSDK::Vector2 blockPos;
            blockPos.x = TO_FIXED(this->right + 8) + this->position.x;
            blockPos.y = this->position.y - 655360;

            CLedge *block           = CREATE_ENTITY(CLedge, NULL, blockPos.x, blockPos.y);
            block->velocity.y       = -65536;
            block->animator.frameID = 1;

            blockPos.y += 1310720;
            block                   = CREATE_ENTITY(CLedge, NULL, blockPos.x, blockPos.y);
            block->animator.frameID = 2;
        }
        else
            this->Destroy();
    }
}

// This state goes unused LOL
void CLedge::State_Wait_Respawn(void)
{
    SET_CURRENT_STATE();

    if (!this->CheckOnScreen(NULL)) {
        this->state.Set(&CLedge::State_Ledge);
        this->stateDraw.Set(&CLedge::Draw_Ledge);
        this->noBlocks = this->length;
        this->right    = this->length << 3;
        this->left     = -this->right;
        this->active   = ACTIVE_BOUNDS;
        this->timer    = 0;
    }
}

void CLedge::State_FallingBlock(void)
{
    SET_CURRENT_STATE();

    this->velocity.y += 16384;
    if (this->velocity.y > 0)
        this->position.y += this->velocity.y;
    if (!this->CheckOnScreen(NULL))
        this->Destroy();
}

void CLedge::Draw_Ledge(void)
{
    SET_CURRENT_STATE();

    RSDK::Vector2 pos;
    pos.x = TO_FIXED(this->left) + 524288 + this->position.x;
    pos.y = this->position.y;

    for (int32 i = 0; i < this->noBlocks; i++) {
        this->animator.DrawSprite(&pos, false);
        pos.x += 1048576;
    }
}

void CLedge::Draw_Crumble_Left(void)
{
    SET_CURRENT_STATE();

    RSDK::Vector2 pos;
    pos.x = TO_FIXED(this->right) - 524288 + this->position.x;
    pos.y = this->position.y;

    for (int32 i = 0; i < this->noBlocks; i++) {
        this->animator.DrawSprite(&pos, false);
        pos.x -= 1048576;
    }
}

void CLedge::Draw_FallingBlock(void)
{
    SET_CURRENT_STATE();
    this->animator.DrawSprite(NULL, false);
}

#if GAME_INCLUDE_EDITOR
void CLedge::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 3, true, 0);

    RSDK::Vector2 pos = this->position;
    int32 xOffset     = TO_FIXED(-(this->animator.GetFrame(sVars->aniFrames)->width * (this->length / 2)));
    if (this->length % 2 != 1)
        xOffset -= TO_FIXED(this->animator.GetFrame(sVars->aniFrames)->pivotX);
    pos.x += xOffset;
    for (int32 i = 0; i < this->length; i++) {
        this->animator.DrawSprite(&pos, false);
        pos.x += TO_FIXED(this->animator.GetFrame(sVars->aniFrames)->width);
    }
}

void CLedge::EditorLoad(void) { sVars->aniFrames.Load("SSZ/Platform.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void CLedge::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(CLedge); }
#endif

void CLedge::Serialize(void) { RSDK_EDITABLE_VAR(CLedge, VAR_INT32, length); }

} // namespace GameLogic
