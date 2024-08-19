// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Ring Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "Ring.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Ring);

void Ring::Update(void) { this->state.Run(this); }
void Ring::LateUpdate(void) {}
void Ring::StaticUpdate(void) { sVars->ringAnim.Process(); }

void Ring::Draw(void)
{
    if (this->type != RING_SPARKLE)
        sVars->ringAnim.DrawSprite(NULL, false);
    else
        this->animator.DrawSprite(NULL, false);
}

void Ring::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->type = (RingTypes)(VOID_TO_INT(data));
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;

        switch (this->type) {
            case RING_NORMAL: this->state.Set(&Ring::State_Normal); break;
            case RING_LOSE:
                this->state.Set(&Ring::State_Lose);
                this->collisionLayers = StageSetup::sVars->collisionLayers;
                break;
            case RING_SPARKLE:
                this->state.Set(&Ring::State_Sparkle);
                this->drawGroup = 4;
                this->animator.SetAnimation(sVars->aniFrames, 1, true, 0);
                break;
        }
    }
}

void Ring::StageLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Ring.bin", SCOPE_STAGE);
    sVars->ringAnim.SetAnimation(&sVars->aniFrames, 0, true, 0);

    sVars->hitbox.left   = -8;
    sVars->hitbox.top    = -8;
    sVars->hitbox.right  = 8;
    sVars->hitbox.bottom = 8;

    sVars->sfxRing.Get("NexusGlobal/Ring.wav");
}

void Ring::State_Normal(void)
{
    SET_CURRENT_STATE();

    foreach_active(Player, player)
    {
        if (!player->state.Matches(&Player::State_Hurt_Recoil) && this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
            this->type      = RING_SPARKLE;
            this->drawGroup = 4;
            this->animator.SetAnimation(sVars->aniFrames, 1, true, 0);
            this->state.Set(&Ring::State_Sparkle);
            player->rings++;

            if (player->rings > 999)
                player->rings = 999;

            if (player->rings >= player->ringExtraLife) {
                globals->lives++;
                Player::sVars->sfxYes.Play();
                player->ringExtraLife += 100;
                // literally 1984
                if (player->ringExtraLife > 300)
                    player->ringExtraLife = 1000;
            }

            if (sVars->pan) {
                RSDK::Channel channel = sVars->sfxRing.Play();
                channel.SetAttributes(1.0, -1.0, 1.0);
                sVars->pan = 0;
            }
            else {
                RSDK::Channel channel = sVars->sfxRing.Play();
                channel.SetAttributes(1.0, 1.0, 1.0);
                sVars->pan = 1;
            }
        }
    }
}

void Ring::State_Lose(void)
{
    SET_CURRENT_STATE();

    this->velocity.y += 6144;

    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;

    if (this->velocity.y >= 0) {
        if (this->TileCollision(this->collisionLayers, CMODE_FLOOR, this->collisionPlane, 0, 8, true)) {
            this->velocity.y = -this->velocity.y;

            this->velocity.y += 32768;

            if (this->velocity.y > -65536)
                this->velocity.y = -65536;
        }
    }

    if (++this->timer == 256) {
        this->Destroy();
        return;
    }

    foreach_active(Player, player)
    {
        if (!player->state.Matches(&Player::State_Hurt_Recoil) && this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
            this->type      = RING_SPARKLE;
            this->drawGroup = 4;
            this->animator.SetAnimation(sVars->aniFrames, 1, true, 0);
            this->state.Set(&Ring::State_Sparkle);
            this->velocity = { 0, 0 };
            player->rings++;

            if (player->rings > 999)
                player->rings = 999;

            if (player->rings >= player->ringExtraLife) {
                globals->lives++;
                // Fun fact: lose ring doesn't play the 1UP sfx LOL
                player->ringExtraLife += 100;
                // literally 1984
                if (player->ringExtraLife > 300)
                    player->ringExtraLife = 1000;
            }

            if (sVars->pan) {
                RSDK::Channel channel = sVars->sfxRing.Play();
                channel.SetAttributes(1.0, -1.0, 1.0);
                sVars->pan = 0;
            }
            else {
                RSDK::Channel channel = sVars->sfxRing.Play();
                channel.SetAttributes(1.0, 1.0, 1.0);
                sVars->pan = 1;
            }
        }
    }
}

void Ring::State_Sparkle(void)
{
    SET_CURRENT_STATE();

    this->animator.Process();
    if (this->animator.frameID == 4)
        this->Destroy();
}

#if GAME_INCLUDE_EDITOR
void Ring::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, this->type == RING_SPARKLE, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Ring::EditorLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Ring.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Ring::sVars, type);
    RSDK_ENUM_VAR("Normal", RING_NORMAL);
    RSDK_ENUM_VAR("Lose", RING_LOSE);
    RSDK_ENUM_VAR("Sparkle", RING_SPARKLE);
}
#endif

#if RETRO_REV0U
void Ring::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Ring); }
#endif

void Ring::Serialize(void) { RSDK_EDITABLE_VAR(Ring, VAR_ENUM, type); }

} // namespace GameLogic
