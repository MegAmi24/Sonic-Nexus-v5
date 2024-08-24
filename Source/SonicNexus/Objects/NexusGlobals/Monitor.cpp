// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Monitor Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Monitor.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Monitor);

void Monitor::Update(void) { this->state.Run(this); }
void Monitor::LateUpdate(void) {}
void Monitor::StaticUpdate(void) {}

void Monitor::Draw(void)
{
    this->animator.DrawSprite(NULL, false);

    if (this->type && this->iconAnim.animationID)
        this->iconAnim.DrawSprite(&this->iconPos, false);
}

void Monitor::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        this->animator.SetAnimation(sVars->aniFrames, 0, true, this->type);
        this->active          = ACTIVE_BOUNDS;
        this->updateRange.x   = TO_FIXED(64);
        this->updateRange.y   = TO_FIXED(128);
        this->drawGroup       = 3;
        this->collisionLayers = StageSetup::sVars->collisionLayers;
        this->state.Set(&Monitor::State_Idle);
    }
}

void Monitor::StageLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Monitor.bin", SCOPE_GLOBAL);

    sVars->sfxDestroy.Get("NexusGlobal/Destroy.wav");
    sVars->sfxBlueShield.Get("SSZ/BlueShield.wav");

    sVars->hitbox.left   = -15;
    sVars->hitbox.top    = -16;
    sVars->hitbox.right  = 15;
    sVars->hitbox.bottom = 16;
}

void Monitor::State_Idle(void)
{
    SET_CURRENT_STATE();

    if (this->timer < 2)
        this->timer = Math::Rand(0, 16);
    else
        this->timer--;

    if (this->timer == 4 || this->timer == 5)
        this->animator.frameID = 0;
    else
        this->animator.frameID = this->type;

    foreach_active(Player, player)
    {
        if (player->velocity.y >= 0) {
            if (player->animator.animationID == Player::ANI_JUMPING) {
                if (this->CheckCollisionTouchBox(&sVars->hitbox, player, player->outerbox)) {
                    CREATE_ENTITY(Explosion, NULL, this->position.x, this->position.y);

                    player->velocity.y = -player->velocity.y;

                    this->state.Set(&Monitor::State_Powerup_Rise);
                    this->active = ACTIVE_NORMAL;

                    this->iconAnim.SetAnimation(&sVars->aniFrames, 1, true, this->type);
                    this->animator.frameID = 7;

                    this->iconPos = this->position;
                    this->pSpeed  = -229376;
                    this->target  = player;

                    sVars->sfxDestroy.Play();
                }
            }
            else
                player->BoxCollision(this, &sVars->hitbox);
        }
        else if (player->BoxCollision(this, &sVars->hitbox) == C_BOTTOM) {
            this->falling    = true;
            this->velocity.y = -131072;
        }
    }

    if (this->falling) {
        this->velocity.y += 14336;
        this->position.y += this->velocity.y;

        if (this->velocity.y >= 0 && this->TileCollision(this->collisionLayers, this->collisionMode, this->collisionPlane, 0, TO_FIXED(16), true)) {
            this->velocity.y = 0;
            this->falling    = false;
        }
    }
}

void Monitor::State_Powerup_Rise(void)
{
    SET_CURRENT_STATE();

    if (this->pSpeed < 0)
        this->iconPos.y += this->pSpeed;

    this->pSpeed += 8192;

    if (!this->pSpeed) {
        this->state.Set(&Monitor::State_Powerup_Show);
        this->timer = 0;

        switch (this->type) {
            case MONITOR_RINGS:
                this->target->rings += 10;

                if (Ring::sVars->pan) {
                    RSDK::Channel channel = Ring::sVars->sfxRing.Play();
                    channel.SetAttributes(1.0, -1.0, 1.0);
                    Ring::sVars->pan = 0;
                }
                else {
                    RSDK::Channel channel = Ring::sVars->sfxRing.Play();
                    channel.SetAttributes(1.0, 1.0, 1.0);
                    Ring::sVars->pan = 1;
                }
                break;
            case MONITOR_BLUESHIELD:
                sVars->sfxBlueShield.Play();
                this->target->shield = Player::SHIELD_BLUE;
                if (RSDK_GET_ENTITY_GEN(SLOT_POWERUP1)->classID != Invincibility::sVars->classID && GameObject::Find("BlueShield"))
                    GameObject::Reset(SLOT_POWERUP1, BlueShield::sVars->classID, this->target->Slot());
                break;
            case MONITOR_INVINCIBILITY:
                GameObject::Reset(SLOT_POWERUP1, Invincibility::sVars->classID, this->target->Slot());
                this->target->invincibility = 1080;
                Music::Play(Music::TRACK_INVINCIBILITY);
                break;
            case MONITOR_SPEEDSHOES:
                Music::Play(Music::TRACK_SPEEDSHOES);
                this->target->stats.acceleration <<= 1;
                this->target->stats.airAcceleration <<= 1;
                this->target->stats.topSpeed <<= 1;
                this->target->speedShoes = 1320;
                break;
            case MONITOR_EXTRALIFE:
                globals->lives++;
                Player::sVars->sfxYes.Play();
                break;
            case MONITOR_ROBOTNIK:
                if (!this->target->invincibility) {
                    this->target->state.Set(&Player::State_Getting_Hurt);
                    if (this->target->position.x > this->position.x)
                        this->target->groundVel = 131072;
                    else
                        this->target->groundVel = -131072;
                }
                break;
        }
    }
}

void Monitor::State_Powerup_Show(void)
{
    SET_CURRENT_STATE();

    if (++this->timer == 30) {
        this->timer = 0;
        this->state.Set(&Monitor::State_Broken);
        this->type   = MONITOR_BLANK;
        this->active = ACTIVE_BOUNDS;
    }
}

void Monitor::State_Broken(void) { SET_CURRENT_STATE(); }

#if GAME_INCLUDE_EDITOR
void Monitor::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, this->type);
    this->animator.DrawSprite(NULL, false);
}

void Monitor::EditorLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Monitor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Monitor::sVars, type);
    RSDK_ENUM_VAR("Blank", MONITOR_BLANK);
    RSDK_ENUM_VAR("Rings", MONITOR_RINGS);
    RSDK_ENUM_VAR("Blue Shield", MONITOR_BLUESHIELD);
    RSDK_ENUM_VAR("Invincibility", MONITOR_INVINCIBILITY);
    RSDK_ENUM_VAR("Speed Shoes", MONITOR_SPEEDSHOES);
    RSDK_ENUM_VAR("Extra Life", MONITOR_EXTRALIFE);
    RSDK_ENUM_VAR("Robotnik", MONITOR_ROBOTNIK);
}
#endif

#if RETRO_REV0U
void Monitor::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Monitor); }
#endif

void Monitor::Serialize(void) { RSDK_EDITABLE_VAR(Monitor, VAR_ENUM, type); }

} // namespace GameLogic
