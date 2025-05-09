// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: FadeUp Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "FadeUp.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(FadeUp);

void FadeUp::Update(void)
{
    if (this->blackFade)
        this->blackFade -= 4;
    else {
        this->Reset($(PressStart)->classID, 0);
        return;
    }

	if (this->whiteFade)
        this->whiteFade -= 4;

    if (this->blackFade == 128)
        Music::Play(Music::TRACK_STAGE);

    paletteBank[0].SetLimitedFade(1, 2, this->blackFade, 0, 111);
    paletteBank[0].SetLimitedFade(1, 2, this->whiteFade, 112, 255);
}

void FadeUp::LateUpdate(void) {}
void FadeUp::StaticUpdate(void) {}

void FadeUp::Draw(void)
{
    if (this->blackFade == 896)
        Graphics::FillScreen(RSDK_GET_ENTITY(1, Logo)->state.Matches(&Logo::State_BouncingScale) ? 0xFFFFFF : 0x000000, 0xFF, 0xFF, 0xFF);
}

void FadeUp::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = DRAWGROUP_COUNT - 1;

        this->blackFade = 896;
        this->whiteFade = 510;
    }
}

void FadeUp::StageLoad(void)
{
    GameObject::Reset(1, $(Logo)->classID, Logo::LOGO_BOUNCING_SCALE);
    Music::SetTrack("Nexus/TitleScreen.ogg", Music::TRACK_STAGE, false);
}

#if GAME_INCLUDE_EDITOR
void FadeUp::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 5, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void FadeUp::EditorLoad(void) { sVars->aniFrames.Load("NexusTitle/Title.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void FadeUp::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(FadeUp); }
#endif

void FadeUp::Serialize(void) {}

} // namespace GameLogic
