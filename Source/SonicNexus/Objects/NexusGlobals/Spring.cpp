// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Spring Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "Spring.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Spring);

void Spring::Update(void)
{
    foreach_active(Player, player)
    {
        if (this->flipFlags < SPRING_UP_RIGHT)
            player->BoxCollision(this, &this->boxHitbox);
        if (this->CheckCollisionTouchBox(&this->touchHitbox, player, player->outerbox)) {
            this->timer = 8;
            if (this->velocity.x)
                player->groundVel = this->velocity.x;
            if (this->velocity.y)
                player->velocity.y = this->velocity.y;
            sVars->sfxSpring.Play();

            switch (this->flipFlags) {
                case SPRING_UP:
                case SPRING_UP_RIGHT:
                case SPRING_UP_LEFT:
                    player->state.Set(&Player::State_Air_Movement);
                    player->onGround = false;
                    player->animator.SetAnimation(&player->aniFrames, Player::ANI_BOUNCING, false, 0);
                    player->timer = 0;
                    break;

                case SPRING_RIGHT:
                case SPRING_LEFT: player->collisionMode = 0; break;

                case SPRING_DOWN:
                case SPRING_DOWN_RIGHT:
                case SPRING_DOWN_LEFT:
                    player->state.Set(&Player::State_Air_Movement);
                    player->onGround = false;
                    player->timer    = 0;
                    break;
            }
        }
    }

    if (this->timer)
        this->timer--;

    this->animator.frameID = (this->flipFlags * 2) + (this->timer != 0);
}

void Spring::LateUpdate(void) {}

void Spring::StaticUpdate(void) {}

void Spring::Draw(void)
{
    if (this->timer && this->flipFlags >= SPRING_UP_RIGHT)
        this->baseAnim.DrawSprite(NULL, false);
    this->animator.DrawSprite(NULL, false);
}

void Spring::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(64);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;

        this->animator.SetAnimation(sVars->aniFrames, this->type, true, this->flipFlags * 2);
        if (this->flipFlags >= SPRING_UP_RIGHT)
            this->baseAnim.SetAnimation(sVars->aniFrames, 2, true, this->flipFlags - SPRING_UP_RIGHT);

        int32 springVel = this->type == SPRING_YELLOW ? 655360 : 1048576;

        switch (this->flipFlags) {
            case SPRING_UP:
                this->boxHitbox.left   = -16;
                this->boxHitbox.top    = 0;
                this->boxHitbox.right  = 16;
                this->boxHitbox.bottom = 16;

                this->touchHitbox.left   = -15;
                this->touchHitbox.top    = -2;
                this->touchHitbox.right  = 15;
                this->touchHitbox.bottom = 4;

                this->velocity.y = -springVel;
                break;
            case SPRING_RIGHT:
                this->boxHitbox.left   = -16;
                this->boxHitbox.top    = -16;
                this->boxHitbox.right  = 0;
                this->boxHitbox.bottom = 16;

                this->touchHitbox.left   = -4;
                this->touchHitbox.top    = -15;
                this->touchHitbox.right  = 2;
                this->touchHitbox.bottom = 15;

                this->velocity.x = springVel;
                break;
            case SPRING_LEFT:
                this->boxHitbox.left   = 0;
                this->boxHitbox.top    = -16;
                this->boxHitbox.right  = 16;
                this->boxHitbox.bottom = 16;

                this->touchHitbox.left   = -2;
                this->touchHitbox.top    = -15;
                this->touchHitbox.right  = 4;
                this->touchHitbox.bottom = 15;

                this->velocity.x = -springVel;
                break;
            case SPRING_DOWN:
                this->boxHitbox.left   = -16;
                this->boxHitbox.top    = -16;
                this->boxHitbox.right  = 16;
                this->boxHitbox.bottom = 0;

                this->touchHitbox.left   = -15;
                this->touchHitbox.top    = -4;
                this->touchHitbox.right  = 15;
                this->touchHitbox.bottom = 2;

                this->velocity.y = springVel;
                break;
            case SPRING_UP_RIGHT:
                this->touchHitbox.left   = -12;
                this->touchHitbox.top    = -12;
                this->touchHitbox.right  = 12;
                this->touchHitbox.bottom = 12;

                this->velocity.x = springVel;
                this->velocity.y = -springVel;
                break;
            case SPRING_UP_LEFT:
                this->touchHitbox.left   = -12;
                this->touchHitbox.top    = -12;
                this->touchHitbox.right  = 12;
                this->touchHitbox.bottom = 12;

                this->velocity.x = -springVel;
                this->velocity.y = -springVel;
                break;
            case SPRING_DOWN_RIGHT:
                this->touchHitbox.left   = -12;
                this->touchHitbox.top    = -12;
                this->touchHitbox.right  = 12;
                this->touchHitbox.bottom = 12;

                this->velocity.x = springVel;
                this->velocity.y = springVel;
                break;
            case SPRING_DOWN_LEFT:
                this->touchHitbox.left   = -12;
                this->touchHitbox.top    = -12;
                this->touchHitbox.right  = 12;
                this->touchHitbox.bottom = 12;

                this->velocity.x = -springVel;
                this->velocity.y = springVel;
                break;
        }
    }
}

void Spring::StageLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Spring.bin", SCOPE_GLOBAL);

    sVars->sfxSpring.Get("NexusGlobal/Spring.wav");
}

#if GAME_INCLUDE_EDITOR
void Spring::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, this->type, true, this->flipFlags * 2);
    this->animator.DrawSprite(NULL, false);
}

void Spring::EditorLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Spring.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Spring::sVars, type);
    RSDK_ENUM_VAR("Yellow", SPRING_YELLOW);
    RSDK_ENUM_VAR("Red", SPRING_RED);

    RSDK_ACTIVE_VAR(Spring::sVars, flipFlags);
    RSDK_ENUM_VAR("Up", SPRING_UP);
    RSDK_ENUM_VAR("Right", SPRING_RIGHT);
    RSDK_ENUM_VAR("Left", SPRING_LEFT);
    RSDK_ENUM_VAR("Down", SPRING_DOWN);
    RSDK_ENUM_VAR("Up-Right", SPRING_UP_RIGHT);
    RSDK_ENUM_VAR("Up-Left", SPRING_UP_LEFT);
    RSDK_ENUM_VAR("Down-Right", SPRING_DOWN_RIGHT);
    RSDK_ENUM_VAR("Down-Left", SPRING_DOWN_LEFT);
}
#endif

#if RETRO_REV0U
void Spring::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Spring); }
#endif

void Spring::Serialize(void)
{
    RSDK_EDITABLE_VAR(Spring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Spring, VAR_ENUM, flipFlags);
}

} // namespace GameLogic
