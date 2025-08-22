// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: BlueShield Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "BlueShield.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(BlueShield);

void BlueShield::Update(void) {}
void BlueShield::LateUpdate(void) { this->animator.Process(); }
void BlueShield::StaticUpdate(void) {}
void BlueShield::Draw(void) { this->animator.DrawSprite(&this->parent->position, false); }

void BlueShield::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->active        = ACTIVE_NORMAL;
        this->drawGroup     = 4;
        this->inkEffect     = INK_BLEND;
        this->parent        = RSDK_GET_ENTITY(data ? VOID_TO_INT(data) : SLOT_PLAYER1, Player);
        this->position      = this->parent->position;
        this->shieldValue   = 1;
    }
}

void BlueShield::StageLoad(void) { sVars->aniFrames.Load("SSZ/Shield.bin", SCOPE_GLOBAL); }

#if GAME_INCLUDE_EDITOR
void BlueShield::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void BlueShield::EditorLoad(void) { sVars->aniFrames.Load("SSZ/Shield.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void BlueShield::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(BlueShield); }
#endif

void BlueShield::Serialize(void) {}

} // namespace GameLogic
