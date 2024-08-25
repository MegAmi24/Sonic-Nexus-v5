// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Fan Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Fan.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Fan);

void Fan::Update(void)
{
    foreach_active(Player, player)
    {
        if (player->mode == Player::PLAYERMODE_PARACHUTE && this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
            if (player->velocity.y > -262144)
                player->velocity.y -= 12288;
        }
    }

    this->animator.Process();
}

void Fan::LateUpdate(void) {}
void Fan::StaticUpdate(void) {}
void Fan::Draw(void) { this->animator.DrawSprite(NULL, false); }

void Fan::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->animator.SetAnimation(sVars->aniFrames, 1, true, 0);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(256);
        this->drawGroup     = 4;
    }
}

void Fan::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/Parachute.bin", SCOPE_GLOBAL);

    sVars->hitbox.left   = -32;
    sVars->hitbox.top    = -480;
    sVars->hitbox.right  = 32;
    sVars->hitbox.bottom = 8;
}

#if GAME_INCLUDE_EDITOR
void Fan::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 1, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Fan::EditorLoad(void) { sVars->aniFrames.Load("SSZ/Parachute.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void Fan::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Fan); }
#endif

void Fan::Serialize(void) {}

} // namespace GameLogic
