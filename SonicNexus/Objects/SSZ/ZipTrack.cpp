// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: ZipTrack Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "ZipTrack.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(ZipTrack);

void ZipTrack::Update(void) {}
void ZipTrack::LateUpdate(void) {}
void ZipTrack::StaticUpdate(void) {}
void ZipTrack::Draw(void) { this->animator.DrawSprite(NULL, false); }

void ZipTrack::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->track = (ZipTrackTypes)(VOID_TO_INT(data));
        this->animator.SetAnimation(sVars->aniFrames, 1, true, this->track);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;
    }
}

void ZipTrack::StageLoad(void) { sVars->aniFrames.Load("SSZ/ZipLine.bin", SCOPE_GLOBAL); }

#if GAME_INCLUDE_EDITOR
void ZipTrack::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 1, true, this->track);
    this->animator.DrawSprite(NULL, false);
}

void ZipTrack::EditorLoad(void)
{
    sVars->aniFrames.Load("SSZ/ZipLine.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR($(ZipTrack), track);
    RSDK_ENUM_VAR("Track 1", ZIPTRACK_TRACK1);
    RSDK_ENUM_VAR("Track 2", ZIPTRACK_TRACK2);
    RSDK_ENUM_VAR("Track 3", ZIPTRACK_TRACK3);
    RSDK_ENUM_VAR("Track 4", ZIPTRACK_TRACK4);
    RSDK_ENUM_VAR("Track 5", ZIPTRACK_TRACK5);
    RSDK_ENUM_VAR("Track 6", ZIPTRACK_TRACK6);
}
#endif

#if RETRO_REV0U
void ZipTrack::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(ZipTrack); }
#endif

void ZipTrack::Serialize(void) { RSDK_EDITABLE_VAR(ZipTrack, VAR_ENUM, track); }

} // namespace GameLogic
