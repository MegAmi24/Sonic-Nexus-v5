// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: SignPost Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "SignPost.hpp"
#include "Camera.hpp"
#include "Music.hpp"
#include "Ring.hpp"
#include "ActFinish.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(SignPost);

void SignPost::Update(void) { this->state.Run(this); }
void SignPost::LateUpdate(void) {}
void SignPost::StaticUpdate(void) {}

void SignPost::Draw(void)
{
    this->poleAnim.DrawSprite(NULL, false);
    this->signAnim.DrawSprite(NULL, false);
}

void SignPost::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;

        this->state.Set(&SignPost::State_Waiting);
        this->poleAnim.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->signAnim.SetAnimation(sVars->aniFrames, 1, true, 0);
    }
}

void SignPost::StageLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/SignPost.bin", SCOPE_GLOBAL);
    sVars->sfxSignPost.Get("NexusGlobal/SignPost.wav");
}

void SignPost::State_Waiting(void)
{
    SET_CURRENT_STATE();

    this->signAnim.frameID = 0;

    $(Camera)->newBoundary1.x = FROM_FIXED(this->position.x) - screenInfo->center.x;
    $(Camera)->newBoundary2.x = $(Camera)->newBoundary1.x + screenInfo->size.x;

    $(Camera)->newBoundary2.y = FROM_FIXED(this->position.y) + 80;
    $(Camera)->newBoundary1.y = $(Camera)->newBoundary2.y - screenInfo->size.y;

    foreach_active(Player, player)
    {
        if (player->position.x > this->position.x) {
            sceneInfo->timeEnabled = false;
            this->state.Set(&SignPost::State_Spinning);
            this->noSpins = 10;
            sVars->sfxSignPost.Play();
            break;
        }
    }
}

void SignPost::State_Spinning(void)
{
    SET_CURRENT_STATE();

    Music::SetVolume($(Music)->volume - 0.01f);

    int32 prevFrame = this->signAnim.frameID;
    this->signAnim.Process();
    if (this->signAnim.frameID != prevFrame) {
        if (this->signAnim.frameID == 4) {
            RSDK::Vector2 sparklePos;
            sparklePos.x = TO_FIXED(Math::Rand(0, 48)) + this->position.x - 1572864;
            sparklePos.y = TO_FIXED(Math::Rand(0, 32)) + this->position.y - 1835008;

            CREATE_ENTITY(Ring, Ring::RING_SPARKLE, sparklePos.x, sparklePos.y);

            if (--this->noSpins == 0) {
                Music::Play(Music::TRACK_LEVELCOMPLETE);

                foreach_active(Player, player)
                {
                    player->controlMode = Player::CONTROLMODE_NONE;
                    player->up          = false;
                    player->down        = false;
                    player->left        = false;
                    player->jumpPress   = false;
                    player->jumpHold    = false;
                }

                this->state.Set(&SignPost::State_Exit);
                GameObject::Reset(SLOT_ACTFINISH, $(ActFinish)->classID, NULL);
            }
            if (this->signAnim.frameID == 0) {
                RSDK::Vector2 sparklePos;
                sparklePos.x = TO_FIXED(Math::Rand(0, 48)) + this->position.x - 1572864;
                sparklePos.y = TO_FIXED(Math::Rand(0, 32)) + this->position.y - 1835008;

                CREATE_ENTITY(Ring, Ring::RING_SPARKLE, sparklePos.x, sparklePos.y);
            }
        }
    }

    foreach_active(Player, player)
    {
        int32 screenBoundary = this->position.x + 20971520;
        if (player->position.x > screenBoundary) {
            player->position.x = screenBoundary;
            player->groundVel  = 0;
            player->velocity.x = 0;
        }
    }
}

void SignPost::State_Exit(void)
{
    SET_CURRENT_STATE();

    foreach_active(Player, player)
    {
        if (player->controlMode == Player::CONTROLMODE_NONE)
            player->right = true;

        int32 screenBoundary = this->position.x + 20971520;
        if (player->position.x > screenBoundary) {
            player->position.x = screenBoundary;
            player->groundVel  = 0;
            player->velocity.x = 0;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void SignPost::EditorDraw(void)
{
    this->poleAnim.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->poleAnim.DrawSprite(NULL, false);
}

void SignPost::EditorLoad(void) { sVars->aniFrames.Load("NexusGlobals/SignPost.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void SignPost::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(SignPost); }
#endif

void SignPost::Serialize(void) {}

} // namespace GameLogic
