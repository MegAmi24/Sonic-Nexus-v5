// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: RLogo Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "RLogo.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(RLogo);

void RLogo::Update(void) { this->state.Run(this); }
void RLogo::LateUpdate(void) {}
void RLogo::StaticUpdate(void) {}

void RLogo::Draw(void)
{
    Graphics::FillScreen(0x000000, 0xFF, 0xFF, 0xFF);

    this->animator.DrawSprite(&this->position, true);

    if (!this->state.Matches(&RLogo::State_Wait) && this->timer)
        Graphics::FillScreen(0x000000, this->timer, this->timer, this->timer);
}

void RLogo::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = 3;

        this->position = { TO_FIXED(screenInfo->center.x), TO_FIXED(screenInfo->center.y) };
        this->state.Set(&RLogo::State_FadeFromBlack);
        this->timer = 256;
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    }
}

void RLogo::StageLoad(void) { sVars->aniFrames.Load("NexusTitle/RetroLogo.bin", SCOPE_GLOBAL); }

void RLogo::State_FadeFromBlack(void)
{
    SET_CURRENT_STATE();

    if (this->timer)
        this->timer -= 4;
    else
        this->state.Set(&RLogo::State_Wait);
}

void RLogo::State_Wait(void)
{
    SET_CURRENT_STATE();

    if (++this->timer == 180) {
        this->timer = 0;
        this->state.Set(&RLogo::State_FadeToBlack);
    }
}

void RLogo::State_FadeToBlack(void)
{
    SET_CURRENT_STATE();

    if (this->timer < 256)
        this->timer += 4;
    else
        this->Reset($(Intro)->classID, nullptr);
}

#if GAME_INCLUDE_EDITOR
void RLogo::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void RLogo::EditorLoad(void) { sVars->aniFrames.Load("NexusTitle/RetroLogo.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void RLogo::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(RLogo); }
#endif

void RLogo::Serialize(void) {}

} // namespace GameLogic
