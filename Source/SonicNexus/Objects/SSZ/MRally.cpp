// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: MRally Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "MRally.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(MRally);

void MRally::Update(void)
{
    this->state.Run(this);

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

void MRally::LateUpdate(void) {}
void MRally::StaticUpdate(void) {}
void MRally::Draw(void) { this->animator.DrawSprite(NULL, false); }

void MRally::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->active          = ACTIVE_BOUNDS;
        this->updateRange.x   = TO_FIXED(64);
        this->updateRange.y   = TO_FIXED(128);
        this->drawGroup       = 3;
        this->collisionLayers = StageSetup::sVars->collisionLayers;
        this->state.Set(&MRally::State_Move_Left);
    }
}

void MRally::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/MRally.bin", SCOPE_GLOBAL);

    sVars->hitbox.left   = -22;
    sVars->hitbox.top    = -8;
    sVars->hitbox.right  = 22;
    sVars->hitbox.bottom = 8;
}

void MRally::State_Move_Left(void)
{
    SET_CURRENT_STATE();

    this->position.x -= 65536;
    this->animator.speed = 30;
    this->animator.Process();

    foreach_active(Player, player)
    {
        if (player->position.x < this->position.x && (this->position.x - player->position.x) < 4194304)
            this->state.Set(&MRally::State_Move_Left_Fast);
    }

    if (!this->TileGrip(this->collisionLayers, this->collisionMode, this->collisionPlane, TO_FIXED(-8), TO_FIXED(16), 10))
        this->state.Set(&MRally::State_Wait_Left);
}

void MRally::State_Move_Right(void)
{
    SET_CURRENT_STATE();

    this->position.x += 65536;
    this->animator.speed = 30;
    this->animator.Process();

    foreach_active(Player, player)
    {
        if (player->position.x > this->position.x && (player->position.x - this->position.x) < 4194304)
            this->state.Set(&MRally::State_Move_Right_Fast);
    }

    if (!this->TileGrip(this->collisionLayers, this->collisionMode, this->collisionPlane, TO_FIXED(8), TO_FIXED(16), 10))
        this->state.Set(&MRally::State_Wait_Right);
}

void MRally::State_Move_Left_Fast(void)
{
    SET_CURRENT_STATE();

    this->position.x -= 131072;
    this->animator.speed = 60;
    this->animator.Process();

    if (!this->TileGrip(this->collisionLayers, this->collisionMode, this->collisionPlane, TO_FIXED(-8), TO_FIXED(16), 10))
        this->state.Set(&MRally::State_Wait_Left);
}

void MRally::State_Move_Right_Fast(void)
{
    SET_CURRENT_STATE();

    this->position.x += 131072;
    this->animator.speed = 60;
    this->animator.Process();

    if (!this->TileGrip(this->collisionLayers, this->collisionMode, this->collisionPlane, TO_FIXED(8), TO_FIXED(16), 10))
        this->state.Set(&MRally::State_Wait_Right);
}

void MRally::State_Wait_Left(void)
{
    SET_CURRENT_STATE();

    if (++this->timer == 30) {
        this->timer = 0;
        this->state.Set(&MRally::State_Move_Right);
        this->animator.SetAnimation(&sVars->aniFrames, 1, false, 0);
    }
}

void MRally::State_Wait_Right(void)
{
    SET_CURRENT_STATE();

    if (++this->timer == 30) {
        this->timer = 0;
        this->state.Set(&MRally::State_Move_Left);
        this->animator.SetAnimation(&sVars->aniFrames, 0, false, 0);
    }
}

#if GAME_INCLUDE_EDITOR
void MRally::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void MRally::EditorLoad(void) { sVars->aniFrames.Load("SSZ/MRally.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void MRally::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(MRally); }
#endif

void MRally::Serialize(void) {}

} // namespace GameLogic
