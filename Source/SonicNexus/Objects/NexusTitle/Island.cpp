// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Island Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Island.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Island);

void Island::Update(void) { this->position.x -= 4096; }
void Island::LateUpdate(void) {}
void Island::StaticUpdate(void) {}
void Island::Draw(void) { this->animator.DrawSprite(NULL, false); }

void Island::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = 3;
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    }
}

void Island::StageLoad(void) { sVars->aniFrames.Load("NexusTitle/Island.bin", SCOPE_GLOBAL); }

#if GAME_INCLUDE_EDITOR
void Island::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Island::EditorLoad(void) { sVars->aniFrames.Load("NexusTitle/Island.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void Island::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Island); }
#endif

void Island::Serialize(void) {}

} // namespace GameLogic
