// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Invincibility Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Invincibility.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Invincibility);

void Invincibility::Update(void) { this->state.Run(this); }
void Invincibility::LateUpdate(void) {}
void Invincibility::StaticUpdate(void) { sVars->animator.Process(); }
void Invincibility::Draw(void) { sVars->animator.DrawSprite(NULL, false); }

void Invincibility::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = 4;
        if (VOID_TO_INT(data) != -1) {
            this->parent   = RSDK_GET_ENTITY(data ? VOID_TO_INT(data) : SLOT_PLAYER1, Player);
            this->position = this->parent->position;
            this->state.Set(&Invincibility::State_Spawner);
        }
        else
            this->state.Set(&Invincibility::State_Child);
    }
}

void Invincibility::StageLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Invincibility.bin", SCOPE_GLOBAL);
    sVars->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
}

void Invincibility::State_Spawner(void)
{
    SET_CURRENT_STATE();

    if (++this->timer > 3) {
        this->timer = 0;
        CREATE_ENTITY(Invincibility, -1, this->parent->position.x, this->parent->position.y);
    }

    this->position = this->parent->position;
}

void Invincibility::State_Child(void)
{
    SET_CURRENT_STATE();

    this->inkEffect = this->timer > 5 ? INK_BLEND : INK_NONE;

    if (++this->timer > 15)
        this->Destroy();
}

#if GAME_INCLUDE_EDITOR
void Invincibility::EditorDraw(void) { sVars->animator.DrawSprite(NULL, false); }

void Invincibility::EditorLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Invincibility.bin", SCOPE_STAGE);
    sVars->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
}
#endif

#if RETRO_REV0U
void Invincibility::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Invincibility); }
#endif

void Invincibility::Serialize(void) {}

} // namespace GameLogic
