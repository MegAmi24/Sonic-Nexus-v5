// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Logo Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Logo.hpp"
#include "Math.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Logo);

void Logo::Update(void) { this->state.Run(this); }
void Logo::LateUpdate(void) {}
void Logo::StaticUpdate(void) {}
void Logo::Draw(void) { this->stateDraw.Run(this); }

void Logo::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = 4;

        this->position.x = TO_FIXED(screenInfo->center.x);
        this->drawFX |= FX_ROTATE;
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->handAnim.SetAnimation(sVars->aniFrames, 1, true, 0);
        this->sonicBlinkAnim.SetAnimation(sVars->aniFrames, 2, true, 0);
        this->tailsBlinkAnim.SetAnimation(sVars->aniFrames, 3, true, 0);
        if (VOID_TO_INT(data) == LOGO_BOUNCING_SCALE) {
            this->position.y = TO_FIXED(LOGO_YPOS);
            this->scale      = { 2048, 2048 };
            this->scaleSpeed = -24;
            this->drawFX |= FX_SCALE;
            this->state.Set(&Logo::State_BouncingScale);
            this->stateDraw.Set(&Logo::Draw_BouncingScale);
        }
        else if (this->state.Matches(StateMachine_None)) {
            this->position.y = TO_FIXED(-320);
            this->state.Set(&Logo::State_BouncingDown);
            this->stateDraw.Set(&Logo::Draw_Normal);
        }
    }
}

void Logo::StageLoad(void)
{
    sVars->aniFrames.Load("NexusTitle/Title.bin", SCOPE_GLOBAL);
    sVars->sfxLand.Get("NTitle/Land.wav");
}

void Logo::State_BouncingDown(void)
{
    SET_CURRENT_STATE();

    if (this->bounceCount < 4) {
        this->velocity.y += 16384;
        this->position.y += this->velocity.y;

        if (this->position.y > TO_FIXED(LOGO_YPOS)) {
            this->position.y = TO_FIXED(LOGO_YPOS);
            this->velocity.y = -(this->velocity.y >> 1);
            this->bounceCount++;
        }
    }

    this->timer += 8;
    if (this->timer > 511)
        this->timer -= 512;

    this->rotation = Sin512(this->timer) >> 4;

    if (--this->sonicBlink < 19) {
        if (this->sonicBlink == 18)
            this->sonicBlinkAnim.SetAnimation(sVars->aniFrames, 2, true, 0);
        this->sonicBlinkAnim.Process();
        if (this->sonicBlinkAnim.frameID == this->sonicBlinkAnim.frameCount - 1)
            this->sonicBlink = Math::Rand(0, 256) + 19;
    }

    if (--this->tailsBlink < 34) {
        if (this->tailsBlink == 33)
            this->tailsBlinkAnim.SetAnimation(sVars->aniFrames, 3, true, 0);
        this->tailsBlinkAnim.Process();
        if (this->tailsBlinkAnim.frameID == this->tailsBlinkAnim.frameCount - 1)
            this->tailsBlink = Math::Rand(0, 256) + 34;
    }
}

void Logo::State_BouncingScale(void)
{
    SET_CURRENT_STATE();

    if (this->bounceCount < 4) {
        this->scaleSpeed -= 2;
        this->scale.x += this->scaleSpeed;
        this->scale.y = this->scale.x;

        if (this->scale.x < 512) {
            this->scale      = { 512, 512 };
            this->scaleSpeed = -(this->scaleSpeed >> 1);
            sVars->sfxLand.Play();
            this->bounceCount++;
        }
    }
    else {
        if (++this->timer > 127) {
            this->timer = 0;

            this->state.Set(&Logo::State_Static);
            this->stateDraw.Set(&Logo::Draw_Normal);
            this->drawFX &= ~FX_SCALE;

            this->sonicBlink = Math::Rand(0, 256) + 19;
            this->tailsBlink = Math::Rand(0, 256) + 34;
        }
    }
}

void Logo::State_Static(void)
{
    SET_CURRENT_STATE();

    this->timer += 8;
    if (this->timer > 511)
        this->timer -= 512;

    this->rotation = Sin512(this->timer) >> 4;

    if (--this->sonicBlink < 19) {
        if (this->sonicBlink == 18)
            this->sonicBlinkAnim.SetAnimation(sVars->aniFrames, 2, true, 0);
        this->sonicBlinkAnim.Process();
        if (this->sonicBlinkAnim.frameID == this->sonicBlinkAnim.frameCount - 1)
            this->sonicBlink = Math::Rand(0, 256) + 19;
    }

    if (--this->tailsBlink < 34) {
        if (this->tailsBlink == 33)
            this->tailsBlinkAnim.SetAnimation(sVars->aniFrames, 3, true, 0);
        this->tailsBlinkAnim.Process();
        if (this->tailsBlinkAnim.frameID == this->tailsBlinkAnim.frameCount - 1)
            this->tailsBlink = Math::Rand(0, 256) + 34;
    }
}

void Logo::Draw_Normal(void)
{
    SET_CURRENT_STATE();

    this->animator.frameID = 1;
    this->animator.DrawSprite(NULL, true);
    this->animator.frameID = 2;
    this->animator.DrawSprite(NULL, true);

    Vector2 pos = { TO_FIXED(screenInfo->center.x - 52), TO_FIXED(LOGO_YPOS - 13) };
    this->handAnim.DrawSprite(&pos, true);

    if (this->sonicBlink < 19)
        this->sonicBlinkAnim.DrawSprite(NULL, true);

    if (this->tailsBlink < 34)
        this->tailsBlinkAnim.DrawSprite(NULL, true);
}

void Logo::Draw_BouncingScale(void)
{
    SET_CURRENT_STATE();

    this->animator.frameID = 0;
    this->animator.DrawSprite(NULL, true);
}

#if GAME_INCLUDE_EDITOR
void Logo::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Logo::EditorLoad(void) { sVars->aniFrames.Load("NexusTitle/Title.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void Logo::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Logo); }
#endif

void Logo::Serialize(void) {}

} // namespace GameLogic
