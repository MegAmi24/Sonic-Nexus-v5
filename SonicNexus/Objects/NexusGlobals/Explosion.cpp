// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Explosion Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Explosion.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Explosion);

void Explosion::Update(void)
{
    this->animator.Process();
    if (this->animator.animationID == 5)
        this->Destroy();
}

void Explosion::LateUpdate(void) {}
void Explosion::StaticUpdate(void) {}

void Explosion::Draw(void) { this->animator.DrawSprite(NULL, false); }

void Explosion::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 4;
    }
}

void Explosion::StageLoad(void) { sVars->aniFrames.Load("NexusGlobals/Explosion.bin", SCOPE_GLOBAL); }

#if GAME_INCLUDE_EDITOR
void Explosion::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Explosion::EditorLoad(void) { sVars->aniFrames.Load("NexusGlobals/Explosion.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void Explosion::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Explosion); }
#endif

void Explosion::Serialize(void) {}

} // namespace GameLogic
