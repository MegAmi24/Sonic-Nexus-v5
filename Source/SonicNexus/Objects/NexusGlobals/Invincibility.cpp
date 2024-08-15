// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Invincibility Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "Invincibility.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Invincibility);

void Invincibility::Update(void) { this->state.Run(this); }
void Invincibility::LateUpdate(void) {}

void Invincibility::StaticUpdate(void)
{
    if (sceneInfo->state == ENGINESTATE_REGULAR)
        sVars->animator.Process();
}

void Invincibility::Draw(void) { sVars->animator.DrawSprite(NULL, false); }

void Invincibility::Create(void *data)
{
    this->visible   = true;
    this->drawGroup = 4;

    if (!sceneInfo->inEditor) {
        this->parent        = RSDK_GET_ENTITY(VOID_TO_INT(data), Player);
        this->active        = ACTIVE_NORMAL;
        this->position      = this->parent->position;
        if (!data)
            this->state.Set(&Invincibility::State_Spawner);
        else
            this->state.Set(&Invincibility::State_Child);
    }
}

void Invincibility::StageLoad(void)
{
    sVars->aniFrames.Load("SSZ/Invincibility.bin", SCOPE_GLOBAL);
    sVars->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
}

void Invincibility::State_Spawner(void)
{
    if (++this->timer > 3) {
        this->timer = 0;
        CREATE_ENTITY(Invincibility, INT_TO_VOID(1), this->parent->position.x, this->parent->position.y);
    }

    this->position = this->parent->position;
}

void Invincibility::State_Child(void)
{
    this->inkEffect = this->timer > 5 ? INK_BLEND : INK_NONE;

    if (++this->timer > 15)
        this->Destroy();
}

#if GAME_INCLUDE_EDITOR
void Invincibility::EditorDraw(void) { sVars->animator.DrawSprite(NULL, false); }

void Invincibility::EditorLoad(void)
{
    sVars->aniFrames.Load("SSZ/Invincibility.bin", SCOPE_GLOBAL);
    sVars->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
}
#endif

#if RETRO_REV0U
void Invincibility::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Invincibility); }
#endif

void Invincibility::Serialize(void) {}

} // namespace GameLogic
