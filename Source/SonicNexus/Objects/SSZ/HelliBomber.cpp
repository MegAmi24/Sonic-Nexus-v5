// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: HelliBomber Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "HelliBomber.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(HelliBomber);

void HelliBomber::Update(void)
{
    this->state.Run(this);

    if (this->isBullet)
        return;

    this->velocity.y = Sin512($(StageSetup)->oscillation << 2) << 6;

    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;

    this->bladesAnim.Process();

    foreach_active(Player, player)
    {
        if (this->state.Matches(&HelliBomber::State_Idle)) {
            if (this->CheckCollisionTouchBox(&sVars->aggroHitbox, player, player->outerbox)) {
                this->destX = player->position.x;
                if (player->position.x > this->position.x)
                    this->destX += 6291456;
                else
                    this->destX -= 6291456;
                this->state.Set(&HelliBomber::State_Move);
                this->target = player;
            }
        }
        else if (this->state.Matches(&HelliBomber::State_Wait) || this->state.Matches(&HelliBomber::State_Shoot))
            this->direction = this->position.x < player->position.x ? FLIP_X : FLIP_NONE;

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

void HelliBomber::LateUpdate(void) {}
void HelliBomber::StaticUpdate(void) {}

void HelliBomber::Draw(void)
{
    if (!this->isBullet) {
        this->animator.frameID = this->direction;
        this->animator.DrawSprite(NULL, false);
        this->bladesAnim.DrawSprite(NULL, false);
    }
    else
        this->animator.DrawSprite(NULL, false);
}

void HelliBomber::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->isBullet = VOID_TO_INT(data);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;

        if (!this->isBullet) {
            this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
            this->bladesAnim.SetAnimation(sVars->aniFrames, 2, true, 0);
            this->state.Set(&HelliBomber::State_Idle);
        }
        else {
            this->animator.SetAnimation(sVars->aniFrames, 3, true, 0);
            this->state.Set(&HelliBomber::State_Bullet);
        }
    }
}

void HelliBomber::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/HelliBomber.bin", SCOPE_GLOBAL);

    sVars->hitbox.left   = -22;
    sVars->hitbox.top    = -8;
    sVars->hitbox.right  = 22;
    sVars->hitbox.bottom = 8;

    sVars->aggroHitbox.left   = -128;
    sVars->aggroHitbox.top    = -256;
    sVars->aggroHitbox.right  = 128;
    sVars->aggroHitbox.bottom = 256;

    sVars->bulletHitbox.left   = -4;
    sVars->bulletHitbox.top    = -4;
    sVars->bulletHitbox.right  = 4;
    sVars->bulletHitbox.bottom = 4;
}

void HelliBomber::State_Idle(void) { SET_CURRENT_STATE(); }

void HelliBomber::State_Move(void)
{
    SET_CURRENT_STATE();

    this->velocity.x = (this->destX - this->position.x) >> 5;

    if ((this->destX >> 19) == (this->position.x >> 19)) {
        this->state.Set(&HelliBomber::State_Wait);
        this->timer = 0;
    }

    this->direction = this->velocity.x > 0 ? FLIP_X : FLIP_NONE;
}

void HelliBomber::State_Wait(void)
{
    SET_CURRENT_STATE();

    if (++this->timer == 15)
        this->state.Set(&HelliBomber::State_Shoot);
}

void HelliBomber::State_Shoot(void)
{
    SET_CURRENT_STATE();

    this->animator.SetAnimation(&sVars->aniFrames, 1, false, 0);
    if (++this->timer == 90) {
        HelliBomber *bullet = CREATE_ENTITY(HelliBomber, true, this->position.x, this->position.y);
        if (this->direction == FLIP_X)
            bullet->position.x += 524288;
        else
            bullet->position.x -= 524288;
        bullet->position.y += 1179648;

        bullet->velocity.x = (this->target->position.x - bullet->position.x) / 48;
        bullet->velocity.y = (this->target->position.y - bullet->position.y) / 48;
        if (bullet->velocity.y < 0)
            bullet->velocity.y = -bullet->velocity.y;
    }

    if (this->timer == 120) {
        this->animator.SetAnimation(&sVars->aniFrames, 0, false, 0);
        this->state.Set(&HelliBomber::State_Idle);
    }
}

void HelliBomber::State_Bullet(void)
{
    SET_CURRENT_STATE();

    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;

    if (!this->CheckOnScreen(NULL))
        this->Destroy();
    else {
        foreach_all(Player, player)
        {
            if (this->CheckCollisionTouchBox(&sVars->bulletHitbox, player, player->outerbox) && !player->invincibility) {
                player->state.Set(&Player::State_Getting_Hurt);
                if (player->position.x > this->position.x)
                    player->groundVel = 131072;
                else
                    player->groundVel = -131072;
            }
        }

        this->animator.Process();
    }
}

#if GAME_INCLUDE_EDITOR
void HelliBomber::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, this->isBullet ? 3 : 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void HelliBomber::EditorLoad(void) { sVars->aniFrames.Load("SSZ/HelliBomber.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void HelliBomber::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(HelliBomber); }
#endif

void HelliBomber::Serialize(void) { RSDK_EDITABLE_VAR(HelliBomber, VAR_BOOL, isBullet); }

} // namespace GameLogic
