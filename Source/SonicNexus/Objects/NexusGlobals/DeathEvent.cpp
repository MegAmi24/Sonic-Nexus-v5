// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: DeathEvent Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "DeathEvent.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(DeathEvent);

void DeathEvent::Update(void) { this->state.Run(this); }
void DeathEvent::LateUpdate(void) {}
void DeathEvent::StaticUpdate(void) {}

void DeathEvent::Draw(void)
{
    if (this->animator.animationID) {
        this->animator.frameID = this->type;
        this->animator.DrawSprite(&this->leftPos, true);
        this->animator.frameID = 2;
        this->animator.DrawSprite(&this->rightPos, true);
    }

    if (this->timer > 0)
        Graphics::FillScreen(0x000000, this->timer, this->timer, this->timer);
}

void DeathEvent::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->type = (DeathEventTypes)VOID_TO_INT(data);
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = 7; // Using 7 instead of 6 so the fade draws above the HUD

        switch (this->type) {
            case DEATHEVENT_GAMEOVER:
            case DEATHEVENT_TIMEOVER:
                this->leftPos.x = TO_FIXED(-8);
                this->leftPos.y = TO_FIXED(screenInfo->center.y);

                this->rightPos.x = TO_FIXED(screenInfo->size.x + 7);
                this->rightPos.y = TO_FIXED(screenInfo->center.y);

                this->timer = -2880;
                this->state.Set(&DeathEvent::State_GameOver);

                this->animator.SetAnimation(&sVars->aniFrames, 5, true, 0);
                break;
            case DEATHEVENT_FADETOBLACK:
                this->timer = 0;
                this->state.Set(&DeathEvent::State_FadeToBlack);
                break;
        }
    }
}

void DeathEvent::StageLoad(void) { sVars->aniFrames.Load("NexusGlobals/HUD.bin", SCOPE_GLOBAL); }

void DeathEvent::State_GameOver(void)
{
    SET_CURRENT_STATE();

    int32 leftTarget = TO_FIXED(screenInfo->center.x - 11);
    if (this->leftPos.x < leftTarget) {
        this->leftPos.x += TO_FIXED(16);
        if (this->leftPos.x > leftTarget)
            this->leftPos.x = leftTarget;
    }

    int32 rightTarget = TO_FIXED(screenInfo->center.x + 7);
    if (this->rightPos.x > rightTarget) {
        this->rightPos.x -= TO_FIXED(16);
        if (this->rightPos.x < rightTarget)
            this->rightPos.x = rightTarget;
    }

    if (this->timer < 288)
        this->timer += 4;
    else {
        if (this->type == DEATHEVENT_GAMEOVER)
            Stage::SetScene("Presentation Stages", "Intro Screen");
        if (this->type == DEATHEVENT_TIMEOVER) {
            globals->recMilliseconds = 0;
            globals->recSeconds      = 0;
            globals->recMinutes      = 0;
        }
        Stage::LoadScene();
    }

    if (this->timer > 0)
        Music::SetVolume(Music::sVars->volume - 0.02f);
    else {
        ControllerState *controller = &controllerInfo[Input::CONT_ANY];
        if (controller->keyA.press || controller->keyB.press || controller->keyC.press)
            this->timer = 0;
    }
}

void DeathEvent::State_FadeToBlack(void)
{
    SET_CURRENT_STATE();

    if (this->timer < 288) {
        Music::SetVolume(Music::sVars->volume - 0.02f);
        this->timer += 4;
    }
    else
        Stage::LoadScene();
}

#if GAME_INCLUDE_EDITOR
void DeathEvent::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 4);
    this->animator.DrawSprite(NULL, false);
}

void DeathEvent::EditorLoad(void) { sVars->aniFrames.Load("NexusGlobals/Editor.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void DeathEvent::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(DeathEvent); }
#endif

void DeathEvent::Serialize(void) {}

} // namespace GameLogic
