// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: RotatePalette Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "RotatePalette.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(RotatePalette);

void RotatePalette::Update(void) {}
void RotatePalette::LateUpdate(void) {}

void RotatePalette::StaticUpdate(void)
{
    if (++sVars->timer == 4) {
        sVars->timer = 0;
        paletteBank[0].Rotate(128, 131, true);
    }
}

void RotatePalette::Draw(void) {}
void RotatePalette::Create(void *data) {}
void RotatePalette::StageLoad(void) { Music::SetTrack("Nexus/SunsetShore.ogg", Music::TRACK_STAGE, true); }

#if GAME_INCLUDE_EDITOR
void RotatePalette::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void RotatePalette::EditorLoad(void) { sVars->aniFrames.Load("NexusGlobals/Editor.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void RotatePalette::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(RotatePalette); }
#endif

void RotatePalette::Serialize(void) {}

} // namespace GameLogic
