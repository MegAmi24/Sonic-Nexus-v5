// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Intro Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Intro.hpp"
#include "NexusGlobals/Music.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Intro);

void Intro::Update(void) { this->state.Run(this); }
void Intro::LateUpdate(void) {}
void Intro::StaticUpdate(void) {}
void Intro::Draw(void) { Graphics::FillScreen(this->state.Matches(&Intro::State_FadeToWhite) ? 0xFFFFFF : 0x000000, 0xFF, 0xFF, 0xFF); }

void Intro::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = 3;

        this->position = { TO_FIXED(screenInfo->center.x), TO_FIXED(screenInfo->center.y) };
        this->state.Set(&Intro::State_LoadTheVideo);
        this->timer = 512;
    }
}

void Intro::StageLoad(void) { Music::SetTrack("Nexus/Intro.ogg", Music::TRACK_STAGE, false); }

void Intro::State_LoadTheVideo(void)
{
    SET_CURRENT_STATE();

    if (this->timer > 432)
        this->timer -= 2;
    else {
        if (Video::LoadVideo("Intro.ogv", 0.0, VideoSkipCB))
            Music::Play(Music::TRACK_STAGE);
        this->state.Set(&Intro::State_FadeToWhite);
    }
}

void Intro::State_FadeToWhite(void)
{
    SET_CURRENT_STATE();

    Music::Stop();
    Stage::SetScene("Presentation Stages", "Nexus Title");
    Stage::LoadScene();
}

bool32 Intro::VideoSkipCB(void) { return controllerInfo->keyStart.press; }

#if GAME_INCLUDE_EDITOR
void Intro::EditorDraw(void) {}

void Intro::EditorLoad(void) {}
#endif

#if RETRO_REV0U
void Intro::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Intro); }
#endif

void Intro::Serialize(void) {}

} // namespace GameLogic
