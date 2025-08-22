// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: ActFinish Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "ActFinish.hpp"
#include "HUD.hpp"
#include "TitleCard.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(ActFinish);

void ActFinish::Update(void) { this->state.Run(this); }
void ActFinish::LateUpdate(void) {}
void ActFinish::StaticUpdate(void) {}

void ActFinish::Draw(void)
{
    RSDK::Vector2 pos = this->line0Pos;

    // Draw Sonic Text
    this->gotThroughAnim.frameID = 3;
    this->gotThroughAnim.DrawSprite(&pos, true);

    // Draw Got Text
    this->gotThroughAnim.frameID = 0;
    pos.x += TO_FIXED(79);
    this->gotThroughAnim.DrawSprite(&pos, true);

    pos = this->line1Pos;

    // Draw Through Text
    this->gotThroughAnim.frameID = 1;
    this->gotThroughAnim.DrawSprite(&pos, true);

    // Draw Zone Text
    this->gotThroughAnim.frameID = 2;
    pos.x += TO_FIXED(116);
    pos.y += TO_FIXED(12);
    this->gotThroughAnim.DrawSprite(&pos, true);

    // Draw Act No
    pos.x += TO_FIXED(38);
    pos.y -= TO_FIXED(22);
    this->actNumAnim.DrawSprite(&pos, true);

    pos = this->line2Pos;

    // Draw Time Text
    this->resultsAnim.frameID = 0;
    this->resultsAnim.DrawSprite(&pos, true);

    // Draw Bonus Text
    this->resultsAnim.frameID = 2;
    pos.x += TO_FIXED(40);
    this->resultsAnim.DrawSprite(&pos, true);

    // Draw Time Bonus
    pos.x += TO_FIXED(144);
    HUD::DrawNumbers(&this->numbersAnim, pos, this->timeBonus, 5, false);

    pos = this->line3Pos;

    // Draw Ring Text
    this->resultsAnim.frameID = 1;
    this->resultsAnim.DrawSprite(&pos, true);

    // Draw Bonus Text
    this->resultsAnim.frameID = 2;
    pos.x += TO_FIXED(40);
    this->resultsAnim.DrawSprite(&pos, true);

    // Draw Ring Bonus
    pos.x += TO_FIXED(144);
    HUD::DrawNumbers(&this->numbersAnim, pos, this->ringBonus, 5, false);

    pos = this->line4Pos;

    // Draw Total Text
    this->resultsAnim.frameID = 3;
    this->resultsAnim.DrawSprite(&pos, true);

    // Draw Total
    pos.x += TO_FIXED(164);
    HUD::DrawNumbers(&this->numbersAnim, pos, this->total, 5, false);

    if (this->state.Matches(&ActFinish::State_FadeOut) && this->timer)
        Graphics::FillScreen(0x000000, this->timer, this->timer, this->timer);
}

void ActFinish::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->gotThroughAnim.SetAnimation(sVars->aniFrames, 2, true, 3);
        this->actNumAnim.SetAnimation(sVars->aniFrames, 3, true, $(TitleCard)->actID);
        this->resultsAnim.SetAnimation(sVars->aniFrames, 4, true, 0);
        this->numbersAnim.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = 7; // Using 7 instead of 6 so the fade draws above the HUD

        // An extra 16 has been added to each x pos to compensate for init not being a state

        this->line0Pos.x = TO_FIXED(-815);
        this->line0Pos.y = TO_FIXED(63);

        this->line1Pos.x = TO_FIXED(screenInfo->size.x + 825);
        this->line1Pos.y = TO_FIXED(83);

        this->line2Pos.x = TO_FIXED(screenInfo->size.x + 911);
        this->line2Pos.y = TO_FIXED(120);

        this->line3Pos.x = TO_FIXED(screenInfo->size.x + 975);
        this->line3Pos.y = TO_FIXED(136);

        this->line4Pos.x = TO_FIXED(screenInfo->size.x + 1059);
        this->line4Pos.y = TO_FIXED(164);

        this->state.Set(&ActFinish::State_MoveIn);

        switch (sceneInfo->minutes) {
            case 0: this->timeBonus = 10000; break;
            case 1: this->timeBonus = 4000; break;
            case 2: this->timeBonus = 2000; break;
            case 3: this->timeBonus = 1000; break;
            case 4: this->timeBonus = 500; break;
            case 5: this->timeBonus = 100; break;
        }

        this->ringBonus = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->rings * 100;
        this->total     = 0;
    }
}

void ActFinish::StageLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/HUD.bin", SCOPE_GLOBAL);
    sVars->sfxScoreAdd.Get("NexusGlobal/ScoreAdd.wav");
    sVars->sfxScoreTotal.Get("NexusGlobal/ScoreTotal.wav");
}

void ActFinish::State_MoveIn(void)
{
    SET_CURRENT_STATE();

    int32 targetPos = TO_FIXED(screenInfo->center.x - 79);
    if (this->line0Pos.x < targetPos) {
        this->line0Pos.x += TO_FIXED(16);
        if (this->line0Pos.x > targetPos)
            this->line0Pos.x = targetPos;
    }
    else
        this->line0Pos.x = targetPos;

    targetPos = TO_FIXED(screenInfo->center.x - 71);
    if (this->line1Pos.x > targetPos) {
        this->line1Pos.x -= TO_FIXED(16);
        if (this->line1Pos.x < targetPos)
            this->line1Pos.x = targetPos;
    }
    else
        this->line1Pos.x = targetPos;

    targetPos = TO_FIXED(screenInfo->center.x - 97);
    if (this->line2Pos.x > targetPos) {
        this->line2Pos.x -= TO_FIXED(16);
        if (this->line2Pos.x < targetPos)
            this->line2Pos.x = targetPos;
    }
    else
        this->line2Pos.x = targetPos;

    if (this->line3Pos.x > targetPos) {
        this->line3Pos.x -= TO_FIXED(16);
        if (this->line3Pos.x < targetPos)
            this->line3Pos.x = targetPos;
    }
    else
        this->line3Pos.x = targetPos;

    targetPos = TO_FIXED(screenInfo->center.x - 77);
    if (this->line4Pos.x > targetPos) {
        this->line4Pos.x -= TO_FIXED(16);
        if (this->line4Pos.x < targetPos)
            this->line4Pos.x = targetPos;
    }
    else {
        this->line4Pos.x = targetPos;
        this->state.Set(&ActFinish::State_Wait);
        this->timer = 0;
    }
}

void ActFinish::State_Wait(void)
{
    SET_CURRENT_STATE();

    if (++this->timer > 299) {
        this->timer = 0;
        this->state.Set(&ActFinish::State_AddEverying);
    }
}

void ActFinish::State_AddEverying(void)
{
    SET_CURRENT_STATE();

    if (this->ringBonus) {
        this->ringBonus -= 100;
        this->total += 100;
        globals->score += 100;
    }

    if (this->timeBonus) {
        this->timeBonus -= 100;
        this->total += 100;
        globals->score += 100;
    }

    if (this->ringBonus || this->timeBonus) {
        if (++this->timer == 2) {
            sVars->sfxScoreAdd.Play();
            this->timer = 0;
        }
    }
    else {
        this->state.Set(&ActFinish::State_WaitSomeMore);
        this->timer = 0;
        sVars->sfxScoreTotal.Play();
    }
}

void ActFinish::State_WaitSomeMore(void)
{
    SET_CURRENT_STATE();

    if (++this->timer == 160) {
        this->timer = 0;
        this->state.Set(&ActFinish::State_FadeOut);
    }
}

void ActFinish::State_FadeOut(void)
{
    SET_CURRENT_STATE();

    this->timer += 4;
    if (this->timer == 384) {
        this->timer = 0;
        sceneInfo->listPos++;
        globals->starPost = 0;

        if (Stage::CheckValidScene())
            Stage::LoadScene();
        else {
            Stage::SetScene("Presentation Stages", "Intro Screen");
            Stage::LoadScene();
        }
    }
}

#if GAME_INCLUDE_EDITOR
void ActFinish::EditorDraw(void)
{
    this->gotThroughAnim.SetAnimation(sVars->aniFrames, 0, true, 3);
    this->gotThroughAnim.DrawSprite(NULL, false);
}

void ActFinish::EditorLoad(void) { sVars->aniFrames.Load("NexusGlobals/Editor.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void ActFinish::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(ActFinish); }
#endif

void ActFinish::Serialize(void) {}

} // namespace GameLogic
