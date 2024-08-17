// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: StageSetup Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "StageSetup.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(StageSetup);

void StageSetup::Update(void)
{
    if (sceneInfo->timeEnabled && sceneInfo->minutes == 10) {
        sceneInfo->minutes      = 9;
        sceneInfo->seconds      = 59;
        sceneInfo->milliseconds = 99;

        Player::sVars->sfxHurt.Play();
        sceneInfo->timeEnabled = false;
        
        Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player1->groundVel  = 0;
        player1->velocity.y = -425984;
        player1->state.Set(&Player::State_Dying);
        player1->animator.SetAnimation(&player1->aniFrames, Player::ANI_DYING, false, 0);
        player1->tileCollisions  = false;
        player1->interaction     = false;
        player1->camera->enabled = false;
    }
}

void StageSetup::LateUpdate(void)
{
    foreach_active(Player, player) {
        if ((TO_FIXED(player->outerbox->left) + player->position.x) < TO_FIXED(Camera::sVars->boundary1.x)) {
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->position.x = TO_FIXED(Camera::sVars->boundary1.x) - TO_FIXED(player->outerbox->left);
        }
    }
}

void StageSetup::StaticUpdate(void)
{
    if (++sVars->oscillation == 512)
        sVars->oscillation = 0;
}

void StageSetup::Draw(void) {}

void StageSetup::Create(void *data)
{
    if (!sceneInfo->inEditor)
        this->active = ACTIVE_NORMAL;
}

void StageSetup::StageLoad(void)
{
    Player::sVars->pauseEnabled = true;
    foreach_all(StageSetup, stageSetup) { stageSetup->Destroy(); }
    GameObject::Reset(SLOT_STAGESETUP, StageSetup::sVars->classID, NULL);

    // StarPost stuff goes here
}

#if GAME_INCLUDE_EDITOR
void StageSetup::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void StageSetup::EditorLoad(void) { sVars->aniFrames.Load("NexusGlobals/Editor.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void StageSetup::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(StageSetup); }
#endif

void StageSetup::Serialize(void) {}

} // namespace GameLogic
