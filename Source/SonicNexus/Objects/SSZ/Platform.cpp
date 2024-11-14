// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Platform Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Platform.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Platform);

void Platform::Update(void)
{
    if (this->type != PLATFORM_FLOAT) {
        this->angle = StageSetup::sVars->oscillation << 1;
        if (this->flippedOsc)
            this->angle += 256;

        if (this->type == PLATFORM_HORIZONTAL) {
            this->changePos.x = Cos512(this->angle) << 13;
            this->changePos.x += this->position.x;
            this->changePos.x &= 4294901760;
            this->changePos.x -= this->platformPos.x;
        }
        else if (this->type == PLATFORM_VERTICAL) {
            this->changePos.y = Sin512(this->angle) << 13;
            this->changePos.y += this->position.y;
            this->changePos.y &= 4294901760;
            this->changePos.y -= this->platformPos.y;
        }
    }

    if (this->type != PLATFORM_VERTICAL) {
        if (this->pressed) {
            if (this->platformPos.y < 16) {
                this->platformPos.y++;
                if ((this->platformPos.y & 3) == 3)
                    this->changePos.y = TO_FIXED(1);
                else
                    this->changePos.y = 0;
            }
            else
                this->changePos.y = 0;
        }
        else {
            if (this->platformPos.y) {
                this->platformPos.y--;
                if ((this->platformPos.y & 3) == 3)
                    this->changePos.y = TO_FIXED(-1);
                else
                    this->changePos.y = 0;
            }
            else
                this->changePos.y = 0;
        }

        this->pressed = false;
    }

    foreach_active(Player, player)
    {
        RSDK::Vector2 posStore = this->position;
        if (this->type == PLATFORM_HORIZONTAL)
            this->position.x = this->platformPos.x;
        else if (this->type == PLATFORM_VERTICAL)
            this->position.y = this->platformPos.y;

        if (player->PlatformCollision(this, &sVars->hitbox)) {
            if (this->type != PLATFORM_VERTICAL)
                this->pressed = true;
            if (this->type == PLATFORM_HORIZONTAL)
                player->position.x += this->changePos.x;
            player->position.y += this->changePos.y;
        }

        this->position = posStore;
    }

    switch (this->type) {
        case PLATFORM_FLOAT: this->position.y += this->changePos.y; break;
        case PLATFORM_HORIZONTAL:
            this->platformPos.x = Cos512(this->angle) << 13;
            this->platformPos.x += this->position.x;
            this->platformPos.x &= 4294901760;
            this->position.y += this->changePos.y;
            break;
        case PLATFORM_VERTICAL:
            this->platformPos.y = Sin512(this->angle) << 13;
            this->platformPos.y += this->position.y;
            this->platformPos.y &= 4294901760;
            break;
    }
}

void Platform::LateUpdate(void) {}
void Platform::StaticUpdate(void) {}

void Platform::Draw(void)
{
    RSDK::Vector2 pos = this->position;
    if (this->type == PLATFORM_HORIZONTAL)
        pos.x = this->platformPos.x;
    else if (this->type == PLATFORM_VERTICAL)
        pos.y = this->platformPos.y;

    this->animator.DrawSprite(&pos, false);
}

void Platform::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->type = (PlatformTypes)(VOID_TO_INT(data));
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(128);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;
    }
}

void Platform::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/Platform.bin", SCOPE_GLOBAL);

    sVars->hitbox.left   = -32;
    sVars->hitbox.top    = -16;
    sVars->hitbox.right  = 32;
    sVars->hitbox.bottom = 8;
}

#if GAME_INCLUDE_EDITOR
void Platform::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Platform::EditorLoad(void)
{
    sVars->aniFrames.Load("SSZ/Platform.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Platform::sVars, type);
    RSDK_ENUM_VAR("Floating", PLATFORM_FLOAT);
    RSDK_ENUM_VAR("Horizontal", PLATFORM_HORIZONTAL);
    RSDK_ENUM_VAR("Vertical", PLATFORM_VERTICAL);
}
#endif

#if RETRO_REV0U
void Platform::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Platform); }
#endif

void Platform::Serialize(void)
{
    RSDK_EDITABLE_VAR(Platform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Platform, VAR_BOOL, flippedOsc);
}

} // namespace GameLogic
