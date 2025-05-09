// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Spikes Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Spikes.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Spikes);

void Spikes::Update(void)
{
    foreach_active(Player, player)
    {
        player->BoxCollision(this, &sVars->boxHitbox);
        
        if (!player->invincibility && this->CheckCollisionTouchBox(&this->touchHitbox, player, player->outerbox)) {
            player->state.Set(&Player::State_Getting_Hurt);
            switch (this->flipFlags) {
                case SPIKES_UP:
                case SPIKES_DOWN:
                    if (player->position.x > this->position.x)
                        player->groundVel = 131072;
                    else
                        player->groundVel = -131072;
                    break;
                case SPIKES_RIGHT: player->groundVel = 131072; break;
                case SPIKES_LEFT: player->groundVel = -131072; break;
            }
        }
    }
}

void Spikes::LateUpdate(void) {}
void Spikes::StaticUpdate(void) {}

void Spikes::Draw(void) { this->animator.DrawSprite(NULL, false); }

void Spikes::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->flipFlags = (SpikesDirections)(VOID_TO_INT(data));
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;

        this->animator.SetAnimation(sVars->aniFrames, 0, true, this->flipFlags);

        switch (this->flipFlags) {
            case SPIKES_UP:
                this->touchHitbox.left   = -15;
                this->touchHitbox.top    = -18;
                this->touchHitbox.right  = 15;
                this->touchHitbox.bottom = -12;
                break;
            case SPIKES_RIGHT:
                this->touchHitbox.left   = 12;
                this->touchHitbox.top    = -15;
                this->touchHitbox.right  = 18;
                this->touchHitbox.bottom = 15;
                break;
            case SPIKES_LEFT:
                this->touchHitbox.left   = -18;
                this->touchHitbox.top    = -15;
                this->touchHitbox.right  = -12;
                this->touchHitbox.bottom = 15;
                break;
            case SPIKES_DOWN:
                this->touchHitbox.left   = -15;
                this->touchHitbox.top    = 12;
                this->touchHitbox.right  = 15;
                this->touchHitbox.bottom = 18;
                break;
        }
    }
}

void Spikes::StageLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Spikes.bin", SCOPE_GLOBAL);

    sVars->boxHitbox.left   = -16;
    sVars->boxHitbox.top    = -16;
    sVars->boxHitbox.right  = 16;
    sVars->boxHitbox.bottom = 16;
}

#if GAME_INCLUDE_EDITOR
void Spikes::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, this->flipFlags);
    this->animator.DrawSprite(NULL, false);
}

void Spikes::EditorLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Spikes.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR($(Spikes), flipFlags);
    RSDK_ENUM_VAR("Up", SPIKES_UP);
    RSDK_ENUM_VAR("Right", SPIKES_RIGHT);
    RSDK_ENUM_VAR("Left", SPIKES_LEFT);
    RSDK_ENUM_VAR("Down", SPIKES_DOWN);
}
#endif

#if RETRO_REV0U
void Spikes::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Spikes); }
#endif

void Spikes::Serialize(void) { RSDK_EDITABLE_VAR(Spikes, VAR_ENUM, flipFlags); }

} // namespace GameLogic
