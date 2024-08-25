// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: FrogOmatic Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "FrogOmatic.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(FrogOmatic);

void FrogOmatic::Update(void)
{
    this->velocity.y += 12288;

    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;

    if (this->velocity.y >= 0 && this->TileCollision(this->collisionLayers, this->collisionMode, this->collisionPlane, 0, TO_FIXED(20), true)) {
        this->velocity.y = -262144;
        if (++this->bounce == 4) {
            this->bounce     = 0;
            this->velocity.x = -this->velocity.x;
            this->animator.SetAnimation(&sVars->aniFrames, this->velocity.x > 0, false, 0);
        }
    }

    foreach_active(Player, player)
    {
        if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
            switch (player->animator.animationID) {
                case Player::ANI_SPINDASH:
                case Player::ANI_JUMPING:
                    CREATE_ENTITY(Explosion, NULL, this->position.x, this->position.y);
                    this->Destroy();
                    Player::sVars->sfxDestroy.Play();
                    if (player->velocity.y > 0)
                        player->velocity.y = -player->velocity.y;
                    globals->score += 100;
                    break;
                default:
                    if (RSDK_GET_ENTITY_GEN(SLOT_POWERUP1)->classID == Invincibility::sVars->classID) {
                        CREATE_ENTITY(Explosion, NULL, this->position.x, this->position.y);
                        this->Destroy();
                        Player::sVars->sfxDestroy.Play();
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

void FrogOmatic::LateUpdate(void) {}
void FrogOmatic::StaticUpdate(void) {}

void FrogOmatic::Draw(void)
{
    this->animator.frameID = 0;

    if (this->velocity.y > 131072)
        this->animator.frameID = 2;
    else if (this->velocity.y > 32768)
        this->animator.frameID = 1;

    if (this->velocity.y < -196608)
        this->animator.frameID = 2;
    else if (this->velocity.y < -131072)
        this->animator.frameID = 1;

    this->animator.DrawSprite(NULL, false);
}

void FrogOmatic::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->active          = ACTIVE_BOUNDS;
        this->updateRange.x   = TO_FIXED(64);
        this->updateRange.y   = TO_FIXED(128);
        this->drawGroup       = 3;
        this->collisionLayers = StageSetup::sVars->collisionLayers;
        this->velocity.x      = -65536;
    }
}

void FrogOmatic::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/FrogOmatic.bin", SCOPE_GLOBAL);

    sVars->hitbox.left   = -14;
    sVars->hitbox.top    = -14;
    sVars->hitbox.right  = 14;
    sVars->hitbox.bottom = 14;
}

#if GAME_INCLUDE_EDITOR
void FrogOmatic::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 2);
    this->animator.DrawSprite(NULL, false);
}

void FrogOmatic::EditorLoad(void) { sVars->aniFrames.Load("SSZ/FrogOmatic.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void FrogOmatic::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(FrogOmatic); }
#endif

void FrogOmatic::Serialize(void) {}

} // namespace GameLogic
