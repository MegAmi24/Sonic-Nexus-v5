// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: PSwitch Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "PSwitch.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(PSwitch);

void PSwitch::Update(void)
{
    foreach_active(Player, player)
    {
        if (this->type != PSWITCH_LOOP || player->onGround) {
            if (this->CheckCollisionTouchBox(&this->hitbox, player, player->outerbox)) {
                if (player->groundVel > 0)
                    player->collisionPlane = this->type == PSWITCH_A ? 0 : 1;
                else
                    player->collisionPlane = this->type == PSWITCH_A ? 1 : 0;
            }
        }
    }
}

void PSwitch::LateUpdate(void) {}
void PSwitch::StaticUpdate(void) {}
void PSwitch::Draw(void) {}

void PSwitch::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);

        this->hitbox.left   = -12;
        this->hitbox.right  = 12;
        this->hitbox.bottom = (this->height << 3) + 8;
        this->hitbox.top    = -this->hitbox.bottom;
    }
}

void PSwitch::StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void PSwitch::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, this->type == PSWITCH_A ? 7 : 6);

    RSDK::Vector2 pos = this->position;
    int32 yOffset     = TO_FIXED(-(this->animator.GetFrame(sVars->aniFrames)->height * (this->height / 2)));
    if (this->height % 2 != 1)
        yOffset -= TO_FIXED(this->animator.GetFrame(sVars->aniFrames)->pivotY);
    pos.y += yOffset;
    for (int32 i = 0; i < this->height; i++) {
        this->animator.DrawSprite(&pos, false);
        pos.y += TO_FIXED(this->animator.GetFrame(sVars->aniFrames)->height);
    }
}

void PSwitch::EditorLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Editor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR($(PSwitch), type);
    RSDK_ENUM_VAR("Plane A", PSWITCH_A);
    RSDK_ENUM_VAR("Plane B", PSWITCH_B);
    RSDK_ENUM_VAR("Loop Top", PSWITCH_LOOP);
}
#endif

#if RETRO_REV0U
void PSwitch::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(PSwitch); }
#endif

void PSwitch::Serialize(void)
{
    RSDK_EDITABLE_VAR(PSwitch, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(PSwitch, VAR_INT32, height);
}

} // namespace GameLogic
