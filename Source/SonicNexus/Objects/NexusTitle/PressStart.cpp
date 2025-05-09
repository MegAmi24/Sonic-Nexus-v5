// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: PressStart Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "PressStart.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(PressStart);

void PressStart::Update(void) { this->state.Run(this); }

void PressStart::LateUpdate(void)
{
    if (++this->timer > 29)
        this->timer = 0;
}

void PressStart::StaticUpdate(void) {}

void PressStart::Draw(void)
{
    if (this->timer > 14) {
        this->animator.frameID = 0;
        this->animator.DrawSprite(NULL, true);
        this->animator.frameID = 1;
        this->animator.DrawSprite(NULL, true);
    }

    if (this->fadeTimer)
        Graphics::FillScreen(0x000000, this->fadeTimer, this->fadeTimer, this->fadeTimer);
}

void PressStart::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = DRAWGROUP_COUNT - 1;

        this->position = { TO_FIXED(screenInfo->center.x), TO_FIXED(200) };
        this->state.Set(&PressStart::State_AwaitStart);
        this->animator.SetAnimation(sVars->aniFrames, 4, true, 0);
    }
}

void PressStart::StageLoad(void)
{
    sVars->aniFrames.Load("NexusTitle/Title.bin", SCOPE_GLOBAL);
    sVars->sfxEnter.Get("NTitle/Enter.wav");
}

void PressStart::State_AwaitStart(void)
{
    SET_CURRENT_STATE();

    if (controllerInfo->keyStart.press) {
        this->state.Set(&PressStart::State_FadeToBlack);
        sVars->sfxEnter.Play();
    }
}

void PressStart::State_FadeToBlack(void)
{
    SET_CURRENT_STATE();

    Music::SetVolume($(Music)->volume - 0.02f);
    this->fadeTimer += 4;
    if (this->fadeTimer == 384) {
        globals->lives = 3;
        globals->score = 0;
        Music::Stop();
        Stage::SetScene("Regular Stages", "");
        Stage::LoadScene();
    }
}

#if GAME_INCLUDE_EDITOR
void PressStart::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 4, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void PressStart::EditorLoad(void) { sVars->aniFrames.Load("NexusTitle/Title.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void PressStart::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(PressStart); }
#endif

void PressStart::Serialize(void) {}

} // namespace GameLogic
