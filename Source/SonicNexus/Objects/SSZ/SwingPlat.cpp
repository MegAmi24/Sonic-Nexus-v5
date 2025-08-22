// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: SwingPlat Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "SwingPlat.hpp"
#include "Math.hpp"
#include "NexusGlobals/StageSetup.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(SwingPlat);

void SwingPlat::Update(void)
{
    this->angle = (Sin512($(StageSetup)->oscillation << 1) >> 2) + 128;

    int32 height = (this->height + 1) << 4;

    this->changePos.x = (Cos512(this->angle) * height) << 7;
    this->changePos.x += this->position.x;
    this->changePos.x &= 4294901760;
    this->changePos.x -= this->platformPos.x;

    this->changePos.y = (Sin512(this->angle) * height) << 7;
    this->changePos.y += this->position.y;
    this->changePos.y &= 4294901760;
    this->changePos.y -= this->platformPos.y;

    RSDK::Vector2 posStore = this->position;
    this->position         = this->platformPos;

    foreach_active(Player, player)
    {
        if (player->PlatformCollision(this, &sVars->hitbox)) {
            player->position.x += this->changePos.x;
            player->position.y += this->changePos.y;
        }
    }

    this->position = posStore;

    this->platformPos.x = (Cos512(this->angle) * height) << 7;
    this->platformPos.x += this->position.x;
    this->platformPos.x &= 4294901760;

    this->platformPos.y = (Sin512(this->angle) * height) << 7;
    this->platformPos.y += this->position.y;
    this->platformPos.y &= 4294901760;
}

void SwingPlat::LateUpdate(void) {}
void SwingPlat::StaticUpdate(void) {}

void SwingPlat::Draw(void)
{
    this->animator.frameID = 0;
    this->animator.DrawSprite(NULL, false);

    this->animator.frameID = 1;
    int32 height           = 16;
    for (int32 s = 0; s < this->height; s++) {
        RSDK::Vector2 pos;
        pos.x = (Cos512(this->angle) * height) << 7;
        pos.x += this->position.x;

        pos.y = (Sin512(this->angle) * height) << 7;
        pos.y += this->position.y;

        this->animator.DrawSprite(&pos, false);
        height += 16;
    }

    this->animator.frameID = 2;
    this->animator.DrawSprite(&this->platformPos, false);
}

void SwingPlat::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->height = VOID_TO_INT(data);
        this->animator.SetAnimation(sVars->aniFrames, 1, true, 0);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(128);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;
    }
}

void SwingPlat::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/Platform.bin", SCOPE_GLOBAL);

    sVars->hitbox.left   = -24;
    sVars->hitbox.top    = -6;
    sVars->hitbox.right  = 24;
    sVars->hitbox.bottom = 8;
}

#if GAME_INCLUDE_EDITOR
void SwingPlat::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 1, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void SwingPlat::EditorLoad(void) { sVars->aniFrames.Load("SSZ/Platform.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void SwingPlat::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(SwingPlat); }
#endif

void SwingPlat::Serialize(void) { RSDK_EDITABLE_VAR(SwingPlat, VAR_INT32, height); }

} // namespace GameLogic
