// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: StarPost Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "StarPost.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(StarPost);

void StarPost::Update(void) { this->state.Run(this); }
void StarPost::LateUpdate(void) {}
void StarPost::StaticUpdate(void) {}

void StarPost::Draw(void)
{
    this->animator.DrawSprite(NULL, false);

    if (!this->stateDraw.Matches(StateMachine_None))
        this->stateDraw.Run(this);
}

void StarPost::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;
        this->animator.SetAnimation(&sVars->aniFrames, 0, true, !this->state.Matches(StateMachine_None));
        this->starAnim.SetAnimation(&sVars->aniFrames, 1, true, 0);
        if (this->state.Matches(StateMachine_None))
            this->state.Set(&StarPost::State_Normal);
    }
}

void StarPost::StageLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/StarPost.bin", SCOPE_GLOBAL);

    sVars->sfxLampPost.Get("NexusGlobal/LampPost.wav"); // Inconsistent terminology smh

    sVars->hitbox.left   = -8;
    sVars->hitbox.top    = -32;
    sVars->hitbox.right  = 8;
    sVars->hitbox.bottom = 32;
}

void StarPost::State_Normal(void)
{
    foreach_active(Player, player)
    {
        if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
            globals->starPost = this->Slot();

            globals->recMilliseconds = sceneInfo->milliseconds;
            globals->recSeconds      = sceneInfo->seconds;
            globals->recMinutes      = sceneInfo->minutes;

            this->animator.frameID = 1;
            this->state.Set(&StarPost::State_Swinging);
            this->stateDraw.Set(&StarPost::Draw_Swinging);
            this->timer = 384;

            sVars->sfxLampPost.Play();
        }
    }
}

void StarPost::State_Swinging(void)
{
    this->timer += 24;
    if (this->timer > 1408) {
        this->timer = 0;
        this->state.Set(&StarPost::State_Flashing);
        this->stateDraw.Set(&StarPost::Draw_Flashing);
    }
}

void StarPost::State_Flashing(void)
{
    if (++this->timer == 8)
        this->timer = 0;
    this->starAnim.frameID = this->timer > 3;
}

void StarPost::Draw_Swinging(void)
{
    RSDK::Vector2 pos;
    pos.x = Math::Cos512(this->timer) * 1280 + this->position.x;
    pos.y = Math::Sin512(this->timer) * 1280 + this->position.y - 917504;

    this->starAnim.DrawSprite(&pos, false);
}

void StarPost::Draw_Flashing(void)
{
    RSDK::Vector2 pos = this->position;
    pos.y             = this->position.y - 1572864;

    this->starAnim.DrawSprite(&pos, false);
}

#if GAME_INCLUDE_EDITOR
void StarPost::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void StarPost::EditorLoad(void) { sVars->aniFrames.Load("NexusGlobals/StarPost.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void StarPost::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(StarPost); }
#endif

void StarPost::Serialize(void) {}

} // namespace GameLogic
