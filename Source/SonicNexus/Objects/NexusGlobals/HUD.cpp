// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: HUD Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "HUD.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(HUD);

void HUD::Update(void) { this->state.Run(this); }
void HUD::LateUpdate(void) {}
void HUD::StaticUpdate(void) {}

void HUD::Draw(void)
{
    // Draw Score
    this->textAnim.frameID = 0;
    this->textAnim.DrawSprite(&this->scorePos, true);
    // Draw Time
    this->textAnim.frameID = 1 + (sceneInfo->minutes == 9 && this->flashingTimer > 7);
    this->textAnim.DrawSprite(&this->timePos, true);
    // Draw Rings
    this->textAnim.frameID = 3 + (!this->parent->rings && this->flashingTimer > 7);
    this->textAnim.DrawSprite(&this->ringPos, true);

    // Draw Seconds Markings
    this->textAnim.frameID = 5;
    RSDK::Vector2 pos      = this->timePos;
    pos.x += TO_FIXED(50);
    this->textAnim.DrawSprite(&pos, true);

    // Draw Score Numbers
    pos = this->scorePos;
    pos.x += TO_FIXED(88);
    DrawNumbers(&this->numbersAnim, pos, globals->score, 6, false);

    // Draw Milliseconds
    pos = this->timePos;
    pos.x += TO_FIXED(88);
    DrawNumbers(&this->numbersAnim, pos, sceneInfo->milliseconds, 2, true);

    // Draw Seconds
    pos = this->timePos;
    pos.x += TO_FIXED(64);
    DrawNumbers(&this->numbersAnim, pos, sceneInfo->seconds, 2, true);

    // Draw Minutes
    pos = this->timePos;
    pos.x += TO_FIXED(40);
    DrawNumbers(&this->numbersAnim, pos, sceneInfo->minutes, 1, true);

    // Draw Rings
    pos = this->ringPos;
    pos.x += TO_FIXED(64);
    DrawNumbers(&this->numbersAnim, pos, this->parent->rings, 3, false);

    // Draw Life Icon
    this->lifeAnim.DrawSprite(&this->lifePos, true);
    pos = this->lifePos;
    pos.x += TO_FIXED(25);
    pos.y += TO_FIXED(5);
    DrawNumbers(&this->numbersAnim, pos, globals->lives, 1, true);
}

void HUD::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = 6;

        if (data) {
            this->state.Set(&HUD::State_Enter);
            this->scorePos.x = TO_FIXED(-113);
            this->timePos.x  = TO_FIXED(-145);
            this->ringPos.x  = TO_FIXED(-177);
            this->lifePos.x  = TO_FIXED(-209);
        }
        else
            this->state.Set(&HUD::State_Normal);

        this->scorePos.y = TO_FIXED(7);
        this->timePos.y  = TO_FIXED(23);
        this->ringPos.y  = TO_FIXED(39);
        this->lifePos.y  = TO_FIXED(215);

        this->textAnim.SetAnimation(&sVars->aniFrames, 1, true, 0);
        this->numbersAnim.SetAnimation(&sVars->aniFrames, 0, true, 0);

        this->parent = RSDK_GET_ENTITY(data ? VOID_TO_INT(data) - 1 : SLOT_PLAYER1, Player);
        this->lifeAnim.SetAnimation(&this->parent->aniFrames, Player::ANI_LIFEICON, true, 0);
    }
}

void HUD::StageLoad(void) { sVars->aniFrames.Load("NexusGlobals/HUD.bin", SCOPE_GLOBAL); }

void HUD::State_Normal(void)
{
    SET_CURRENT_STATE();

    if (!this->parent->rings || sceneInfo->minutes == 9) {
        if (++this->flashingTimer == 16)
            this->flashingTimer = 0;
    }
}

void HUD::State_Enter(void)
{
    SET_CURRENT_STATE();

    if (this->scorePos.x < TO_FIXED(15))
        this->scorePos.x += TO_FIXED(16);

    if (this->timePos.x < TO_FIXED(15))
        this->timePos.x += TO_FIXED(16);

    if (this->ringPos.x < TO_FIXED(15))
        this->ringPos.x += TO_FIXED(16);

    if (this->lifePos.x < TO_FIXED(15))
        this->lifePos.x += TO_FIXED(16);
    else {
        this->state.Set(&HUD::State_Normal);
        sceneInfo->timeEnabled = true;
    }
}

void HUD::DrawNumbers(RSDK::Animator *animator, RSDK::Vector2 drawPos, int32 value, uint8 digitCount, bool32 allDigits)
{
    int32 i = 10;
    if (allDigits) {
        while (digitCount > 0) {
            animator->frameID = value % i / (i / 10);
            animator->DrawSprite(&drawPos, true);
            drawPos.x -= TO_FIXED(8);
            i *= 10;
            --digitCount;
        }
    }
    else {
        int32 extra = 10;
        if (value)
            extra = 10 * value;
        while (digitCount > 0) {
            if (extra >= i) {
                animator->frameID = value % i / (i / 10);
                animator->DrawSprite(&drawPos, true);
            }
            drawPos.x -= TO_FIXED(8);
            i *= 10;
            --digitCount;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void HUD::EditorDraw(void)
{
    this->textAnim.SetAnimation(sVars->aniFrames, 0, true, 2);
    this->textAnim.DrawSprite(NULL, false);
}

void HUD::EditorLoad(void) { sVars->aniFrames.Load("NexusGlobals/Editor.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void HUD::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(HUD); }
#endif

void HUD::Serialize(void) {}

} // namespace GameLogic
