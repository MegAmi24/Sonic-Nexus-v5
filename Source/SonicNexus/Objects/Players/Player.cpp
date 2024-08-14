// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Sonic Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Player);

void Player::Update(void)
{
    switch (this->propertyValue) {
        case PLAYERMODE_ACTIVE: {
            ProcessPlayerControl(this);
            Main();

            // Small extra added in this port: accessing unused debug mode if enabled through dev menu
            ControllerState *controller = &controllerInfo[this->Slot() + 1];
            if (sceneInfo->debugMode && controller->keyB.press) {
                this->tileCollisions = false;
                this->interaction    = false;
                this->controlMode    = CONTROLMODE_PLAYER1;
                this->propertyValue  = PLAYERMODE_DEBUG;
            }
            else {
                this->state.Run(this);
                ProcessPlayerAnimation(this);

                this->HandleMovement();
            }
            break;
        }
        case PLAYERMODE_INACTIVE: {
            ProcessPlayerControl(this);
            ProcessPlayerAnimation(this);
            Main();

            this->outerbox = this->animator.GetHitbox(0);
            this->innerbox = this->animator.GetHitbox(1);
            this->ProcessMovement(this->outerbox, this->innerbox);
            break;
        }
        case PLAYERMODE_DEBUG: {
            ProcessPlayerControl(this);
            ProcessDebugMode(this);
            this->camera->enabled       = true;
            ControllerState *controller = &controllerInfo[this->Slot() + 1];
            if (controller->keyB.press) {
                this->tileCollisions = true;
                this->interaction    = true;
                this->controlMode    = CONTROLMODE_PLAYER1;
                this->propertyValue  = PLAYERMODE_ACTIVE;
            }
            break;
        }
    }
}

void Player::LateUpdate(void)
{
    ControllerState *controller = &controllerInfo[this->Slot() + 1];
    if (sceneInfo->state == ENGINESTATE_REGULAR && controller->keyStart.press) {
        Stage::SetEngineState(ENGINESTATE_FROZEN);
        Music::Pause();
    }
    if (this->animator.animationID != this->animator.prevAnimationID && this->animator.animationID != this->animCheck) {
        if (this->animator.animationID == ANI_JUMPING)
            this->position.y += (this->normalbox->bottom - this->jumpbox->bottom) << 16;
        if (this->animator.prevAnimationID == ANI_JUMPING)
            this->position.y -= (this->normalbox->bottom - this->jumpbox->bottom) << 16;
        this->animCheck = this->animator.animationID;
    }
}

void Player::StaticUpdate(void)
{
    if (sVars->frameAdvance)
        Stage::SetEngineState(ENGINESTATE_FROZEN);

    if (sceneInfo->state == ENGINESTATE_FROZEN) {
        sVars->frameAdvance = false;

        ControllerState *controller = &controllerInfo[Input::CONT_ANY];
        if (controller->keyStart.press) {
            Stage::SetEngineState(ENGINESTATE_REGULAR);
            Music::Resume();
        }
        else if (controller->keyC.press) {
            sVars->frameAdvance = true;
            Stage::SetEngineState(ENGINESTATE_REGULAR);
        }
    }
}

void Player::Draw(void) { this->animator.DrawSprite(NULL, false); }

void Player::Create(void *data)
{
    this->aniFrames = sVars->sonicFrames;
    this->camera    = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    analogStickInfoL[this->Slot() + 1].deadzone = 0.3f;

    this->state.Set(&Player::State_Normal_Ground_Movement);
    this->drawGroup      = 4;
    this->active         = ACTIVE_NORMAL;
    this->visible        = true;
    this->onGround       = false;
    this->tileCollisions = true;
    this->interaction    = true;
    this->drawFX         = FX_ROTATE | FX_FLIP;
    SetMovementStats(&this->stats);

    RSDK::SceneLayer fgLow;
    fgLow.Get("FG Low");
    RSDK::SceneLayer fgHigh;
    fgHigh.Get("FG High");
    this->collisionLayers = (1 << fgLow.id) | (1 << fgHigh.id);

    // v2 stores these in the animation files??????? wtf???????
    this->animator.SetAnimation(this->aniFrames, ANI_WALKING, true, 0);
    this->walkingSpeed = this->animator.speed - 20;
    this->animator.SetAnimation(this->aniFrames, ANI_RUNNING, true, 0);
    this->runningSpeed = this->animator.speed;
    this->normalbox    = this->animator.GetHitbox(0);
    this->animator.SetAnimation(this->aniFrames, ANI_JUMPING, true, 0);
    this->jumpingSpeed = this->animator.speed - 48;
    this->jumpbox      = this->animator.GetHitbox(0);

    this->animator.SetAnimation(this->aniFrames, ANI_STOPPED, true, 0);
    this->animCheck = ANI_STOPPED;
}

void Player::StageLoad(void)
{
    Dev::AddViewableVariable("Debug Mode", &sceneInfo->debugMode, Dev::VIEWVAR_BOOL, false, true);

    sVars->active = ACTIVE_ALWAYS;

    sVars->sonicFrames.Load("Sonic/SonicClassic.bin", SCOPE_GLOBAL);

    sVars->sfxJump.Get("NexusGlobal/Jump.wav");
    sVars->sfxRing.Get("NexusGlobal/Ring.wav");
    sVars->sfxLoseRings.Get("NexusGlobal/LoseRings.wav");
    sVars->sfxHurt.Get("NexusGlobal/Hurt.wav");
    sVars->sfxSpin.Get("NexusGlobal/Spin.wav");
    sVars->sfxCharge.Get("NexusGlobal/Charge.wav");
    sVars->sfxRelease.Get("NexusGlobal/Release.wav");
    sVars->sfxSkidding.Get("NexusGlobal/Skidding.wav");
    sVars->sfxDestroy.Get("NexusGlobal/Destroy.wav");
    sVars->sfxBossHit.Get("NexusGlobal/BossHit.wav");
    sVars->sfxYes.Get("NexusGlobal/Yes.wav");

    foreach_all(Player, player) player->Copy(RSDK_GET_ENTITY_GEN(SLOT_PLAYER1), true);

    sVars->upBuffer        = false;
    sVars->downBuffer      = false;
    sVars->leftBuffer      = false;
    sVars->rightBuffer     = false;
    sVars->jumpPressBuffer = false;
    sVars->jumpHoldBuffer  = false;
}

// Extra Entity Functions

void Player::ProcessPlayerControl(Player *player)
{
    if (player->controlMode == CONTROLMODE_NONE) {
        sVars->upBuffer <<= 1;
        sVars->upBuffer |= (uint8)player->up;
        sVars->downBuffer <<= 1;
        sVars->downBuffer |= (uint8)player->down;
        sVars->leftBuffer <<= 1;
        sVars->leftBuffer |= (uint8)player->left;
        sVars->rightBuffer <<= 1;
        sVars->rightBuffer |= (uint8)player->right;
        sVars->jumpPressBuffer <<= 1;
        sVars->jumpPressBuffer |= (uint8)player->jumpPress;
        sVars->jumpHoldBuffer <<= 1;
        sVars->jumpHoldBuffer |= (uint8)player->jumpHold;
    }
    else if (player->controlMode == CONTROLMODE_DELAY) {
        player->up        = sVars->upBuffer >> 15;
        player->down      = sVars->downBuffer >> 15;
        player->left      = sVars->leftBuffer >> 15;
        player->right     = sVars->rightBuffer >> 15;
        player->jumpPress = sVars->jumpPressBuffer >> 15;
        player->jumpHold  = sVars->jumpHoldBuffer >> 15;
    }
    else {
        ControllerState *controller = &controllerInfo[player->Slot() + 1];
        AnalogState *stick          = &analogStickInfoL[player->Slot() + 1];

        player->up    = controller->keyUp.down || stick->keyUp.down || stick->vDelta > 0.3;
        player->down  = controller->keyDown.down || stick->keyDown.down || stick->vDelta < -0.3;
        player->left  = controller->keyLeft.down || stick->keyLeft.down || stick->hDelta < -0.3;
        player->right = controller->keyRight.down || stick->keyRight.down || stick->hDelta > 0.3;

        if (player->left && player->right) {
            player->left  = false;
            player->right = false;
        }
        player->jumpHold  = controller->keyC.down || controller->keyB.down || controller->keyA.down;
        player->jumpPress = controller->keyC.press || controller->keyB.press || controller->keyA.press;
        sVars->upBuffer <<= 1;
        sVars->upBuffer |= (uint8)player->up;
        sVars->downBuffer <<= 1;
        sVars->downBuffer |= (uint8)player->down;
        sVars->leftBuffer <<= 1;
        sVars->leftBuffer |= (uint8)player->left;
        sVars->rightBuffer <<= 1;
        sVars->rightBuffer |= (uint8)player->right;
        sVars->jumpPressBuffer <<= 1;
        sVars->jumpPressBuffer |= (uint8)player->jumpPress;
        sVars->jumpHoldBuffer <<= 1;
        sVars->jumpHoldBuffer |= (uint8)player->jumpHold;
    }
}

void Player::SetMovementStats(PlayerMovementStats *stats)
{
    stats->topSpeed            = 0x60000;
    stats->acceleration        = 0xC00;
    stats->deceleration        = 0xC00;
    stats->airAcceleration     = 0x1800;
    stats->airDeceleration     = 0x600;
    stats->gravityStrength     = 0x3800;
    stats->jumpStrength        = 0x68000;
    stats->rollingDeceleration = 0x2000;
}

void Player::DefaultAirMovement(Player *player)
{
    if (player->velocity.y > -0x40000 && player->velocity.y < 0)
        player->groundVel -= player->groundVel >> 5;

    if (player->groundVel <= -player->stats.topSpeed) {
        if (player->left) {
            player->direction = FLIP_X;
        }
    }
    else {
        if (player->left) {
            player->groundVel -= player->stats.airAcceleration;
            player->direction = FLIP_X;
        }
    }

    if (player->groundVel >= player->stats.topSpeed) {
        if (player->right)
            player->direction = FLIP_NONE;
    }
    else if (player->right) {
        player->direction = FLIP_NONE;
        player->groundVel += player->stats.airAcceleration;
    }
}

void Player::DefaultGravityFalse(Player *player)
{
    player->trackScroll = false;
    player->velocity.x  = player->groundVel * Math::Cos256(player->angle) >> 8;
    player->velocity.y  = player->groundVel * Math::Sin256(player->angle) >> 8;
}

void Player::DefaultGravityTrue(Player *player)
{
    player->trackScroll = true;
    player->velocity.y += player->stats.gravityStrength;
    if (player->velocity.y >= -0x40000) {
        player->timer = 0;
    }
    else if (!player->jumpHold && player->timer > 0) {
        player->timer      = 0;
        player->velocity.y = -0x3C800;
        player->groundVel -= player->groundVel >> 5;
    }
    player->velocity.x = player->groundVel;
    if (player->rotation <= 0 || player->rotation >= 128) {
        if (player->rotation > 127 && player->rotation < 256) {
            player->rotation += 2;
            if (player->rotation > 255) {
                player->rotation = 0;
            }
        }
    }
    else {
        player->rotation -= 2;
        if (player->rotation < 1)
            player->rotation = 0;
    }
}

void Player::DefaultGroundMovement(Player *player)
{
    if ((int32)player->frictionLoss <= 0) {
        if (player->left && player->groundVel > -player->stats.topSpeed) {
            if (player->groundVel <= 0) {
                player->groundVel -= player->stats.acceleration;
                player->skidding = 0;
            }
            else {
                if (player->groundVel > 0x40000)
                    player->skidding = 16;
                if (player->groundVel >= 0x8000) {
                    player->groundVel -= 0x8000;
                }
                else {
                    player->groundVel = -0x8000;
                    player->skidding  = 0;
                }
            }
        }
        if (player->right && player->groundVel < player->stats.topSpeed) {
            if (player->groundVel >= 0) {
                player->groundVel += player->stats.acceleration;
                player->skidding = 0;
            }
            else {
                if (player->groundVel < -0x40000)
                    player->skidding = 16;
                if (player->groundVel <= -0x8000) {
                    player->groundVel += 0x8000;
                }
                else {
                    player->groundVel = 0x8000;
                    player->skidding  = 0;
                }
            }
        }

        if (player->left && player->groundVel <= 0)
            player->direction = FLIP_X;
        if (player->right && player->groundVel >= 0)
            player->direction = FLIP_NONE;

        if (player->left || player->right) {
            switch (player->collisionMode) {
                case CMODE_FLOOR: player->groundVel += Math::Sin256(player->angle) << 13 >> 8; break;
                case CMODE_LWALL:
                    if (player->angle >= 176) {
                        player->groundVel += (Math::Sin256(player->angle) << 13 >> 8);
                    }
                    else {
                        if (player->groundVel < -0x60000 || player->groundVel > 0x60000)
                            player->groundVel += Math::Sin256(player->angle) << 13 >> 8;
                        else
                            player->groundVel += 0x1400 * Math::Sin256(player->angle) >> 8;
                    }
                    break;
                case CMODE_ROOF:
                    if (player->groundVel < -0x60000 || player->groundVel > 0x60000)
                        player->groundVel += Math::Sin256(player->angle) << 13 >> 8;
                    else
                        player->groundVel += 0x1400 * Math::Sin256(player->angle) >> 8;
                    break;
                case CMODE_RWALL:
                    if (player->angle <= 80) {
                        player->groundVel += Math::Sin256(player->angle) << 13 >> 8;
                    }
                    else {
                        if (player->groundVel < -0x60000 || player->groundVel > 0x60000)
                            player->groundVel += Math::Sin256(player->angle) << 13 >> 8;
                        else
                            player->groundVel += 0x1400 * Math::Sin256(player->angle) >> 8;
                    }
                    break;
                default: break;
            }

            if (player->angle > 192) {
                if (player->angle < 226 && !player->left) {
                    if (player->right && player->groundVel < 0x20000) {
                        if (player->groundVel > -0x60000)
                            player->frictionLoss = 30;
                    }
                }
            }
            if (player->angle > 30) {
                if (player->angle < 64 && player->left) {
                    if (!player->right && player->groundVel > -0x20000) {
                        if (player->groundVel < 0x60000)
                            player->frictionLoss = 30;
                    }
                }
            }
        }
        else {
            if (player->groundVel < 0) {
                player->groundVel += player->stats.deceleration;
                if (player->groundVel > 0)
                    player->groundVel = 0;
            }
            if (player->groundVel > 0) {
                player->groundVel -= player->stats.deceleration;
                if (player->groundVel < 0)
                    player->groundVel = 0;
            }
            if (player->groundVel < -0x4000 || player->groundVel > 0x4000)
                player->groundVel += Math::Sin256(player->angle) << 13 >> 8;
            if ((player->angle > 30 && player->angle < 64) || (player->angle > 192 && player->angle < 226)) {
                if (player->groundVel > -0x10000 && player->groundVel < 0x10000)
                    player->frictionLoss = 30;
            }
        }
    }
    else {
        --player->frictionLoss;
        player->groundVel = (Math::Sin256(player->angle) << 13 >> 8) + player->groundVel;
    }
}

void Player::DefaultJumpAction(Player *player)
{
    player->frictionLoss = 0;
    player->onGround     = false;
    player->velocity.x   = (player->groundVel * Math::Cos256(player->angle) + player->stats.jumpStrength * Math::Sin256(player->angle)) >> 8;
    player->velocity.y   = (player->groundVel * Math::Sin256(player->angle) + -player->stats.jumpStrength * Math::Cos256(player->angle)) >> 8;
    player->groundVel    = player->velocity.x;
    player->trackScroll  = true;
    player->animator.SetAnimation(player->aniFrames, ANI_JUMPING, false, 0);
    player->angle         = 0;
    player->collisionMode = CMODE_FLOOR;
    player->timer         = 1;
}

void Player::DefaultRollingMovement(Player *player)
{

    if (player->right && player->groundVel < 0)
        player->groundVel += player->stats.rollingDeceleration;
    if (player->left && player->groundVel > 0)
        player->groundVel -= player->stats.rollingDeceleration;

    if (player->groundVel < 0) {
        player->groundVel += player->stats.airDeceleration;
        if (player->groundVel > 0)
            player->groundVel = 0;
    }
    if (player->groundVel > 0) {
        player->groundVel -= player->stats.airDeceleration;
        if (player->groundVel < 0)
            player->groundVel = 0;
    }
    if ((player->angle < 12 || player->angle > 244) && !player->groundVel)
        player->state.Set(&Player::State_Normal_Ground_Movement);

    if (player->groundVel <= 0) {
        if (Math::Sin256(player->angle) >= 0) {
            player->groundVel += (player->stats.rollingDeceleration * Math::Sin256(player->angle) >> 8);
        }
        else {
            player->groundVel += 0x5000 * Math::Sin256(player->angle) >> 8;
        }
    }
    else if (Math::Sin256(player->angle) <= 0) {
        player->groundVel += (player->stats.rollingDeceleration * Math::Sin256(player->angle) >> 8);
    }
    else {
        player->groundVel += 0x5000 * Math::Sin256(player->angle) >> 8;
    }

    if (player->groundVel > 0x180000)
        player->groundVel = 0x180000;
}

void Player::ProcessDebugMode(Player *player)
{
    if (player->down || player->up || player->right || player->left) {
        if (player->groundVel < 0x100000) {
            player->groundVel += 0xC00;
            if (player->groundVel > 0x100000)
                player->groundVel = 0x100000;
        }
    }
    else {
        player->groundVel = 0;
    }

    ControllerState *controller = &controllerInfo[player->Slot() + 1];
    AnalogState *stick          = &analogStickInfoL[player->Slot() + 1];

    bool32 up    = controller->keyUp.down || stick->keyUp.down || stick->vDelta > 0.3;
    bool32 down  = controller->keyDown.down || stick->keyDown.down || stick->vDelta < -0.3;
    bool32 left  = controller->keyLeft.down || stick->keyLeft.down || stick->hDelta < -0.3;
    bool32 right = controller->keyRight.down || stick->keyRight.down || stick->hDelta > 0.3;

    if (left)
        player->position.x -= player->groundVel;
    if (right)
        player->position.x += player->groundVel;

    if (up)
        player->position.y -= player->groundVel;
    if (down)
        player->position.y += player->groundVel;
}

void Player::ProcessPlayerAnimation(Player *player)
{
    if (player->onGround && !player->state.Matches(&Player::State_Peelout)) {
        int32 speed = (player->jumpingSpeed * abs(player->groundVel) / 6 >> 16) + 48;
        if (speed > 0xF0)
            speed = 0xF0;
        player->jumpAnimSpeed = speed;

        switch (player->animator.animationID) {
            case ANI_WALKING: player->animator.speed = ((uint32)(player->walkingSpeed * abs(player->groundVel) / 6) >> 16) + 20; break;
            case ANI_RUNNING:
                speed = player->runningSpeed * abs(player->groundVel) / 6 >> 16;
                if (speed > 0xF0)
                    speed = 0xF0;
                player->animator.speed = speed;
                break;
            case ANI_PEELOUT:
                speed = player->runningSpeed * abs(player->groundVel) / 6 >> 16;
                if (speed > 0xF0)
                    speed = 0xF0;
                player->animator.speed = speed;
                break;
        }
    }
    if (player->animator.animationID == ANI_JUMPING)
        player->animator.speed = player->jumpAnimSpeed;
    if (player->animator.animationID != player->animator.prevAnimationID && player->animator.animationID != player->animCheck) {
        if (player->animator.animationID == ANI_JUMPING)
            player->position.y += (player->normalbox->bottom - player->jumpbox->bottom) << 16;
        if (player->animator.prevAnimationID == ANI_JUMPING)
            player->position.y -= (player->normalbox->bottom - player->jumpbox->bottom) << 16;
        player->animCheck = player->animator.animationID;
    }
    player->animator.Process();
}

void Player::Main(void)
{
    if (this->speedShoes) {
        this->speedShoes--;
        if (!this->speedShoes) {
            this->stats.acceleration >>= 1;
            this->stats.airAcceleration >>= 1;
            this->stats.topSpeed >>= 1;
            if (Music::CurrentTrack(Music::TRACK_SPEEDSHOES))
                Music::Play(Music::TRACK_STAGE);
        }
    }

    if (this->invincibility) {
        if (!this->state.Matches(&Player::State_Hurt_Recoil) && this->invincibility > 1080) {
            this->invincibility = 120;
            this->flashing      = 3;
        }

        if (this->flashing) {
            this->flashing++;
            if (this->flashing > 8)
                this->flashing = 1;
            this->visible = this->flashing <= 4;
        }

        if (--this->invincibility == 0) {
            this->flashing = false;
            this->visible  = true;
            if (Music::CurrentTrack(Music::TRACK_INVINCIBILITY))
                Music::Play(Music::TRACK_STAGE);
            ApplyShield(this);
        }
    }

    if (!this->state.Matches(&Player::State_Looking_Up) && !this->state.Matches(&Player::State_Looking_Down)) {
        if (this->lookPos > 0)
            this->lookPos -= 2;
        if (this->lookPos < 0)
            this->lookPos += 2;
    }
}

void Player::State_Normal_Ground_Movement(void)
{
    SET_CURRENT_STATE();

    DefaultGroundMovement(this);

    if (!this->onGround) {
        this->state.Set(&Player::State_Air_Movement);
        DefaultGravityTrue(this);
    }
    else {
        DefaultGravityFalse(this);

        if (!this->groundVel) {
            if (this->timer < 240) {
                this->animator.SetAnimation(this->aniFrames, ANI_STOPPED, false, 0);
                this->timer++;
            }
            else
                this->animator.SetAnimation(this->aniFrames, ANI_WAITING, false, 0);

            if (!this->flailing[1]) {
                if (!this->flailing[2]) {
                    if (this->direction == FLIP_X)
                        this->animator.SetAnimation(this->aniFrames, ANI_FLAILINGLEFT, false, 0);
                    else
                        this->animator.SetAnimation(this->aniFrames, ANI_FLAILINGRIGHT, false, 0);
                }

                if (!this->flailing[0]) {
                    if (this->direction == FLIP_NONE)
                        this->animator.SetAnimation(this->aniFrames, ANI_FLAILINGLEFT, false, 0);
                    else
                        this->animator.SetAnimation(this->aniFrames, ANI_FLAILINGRIGHT, false, 0);
                }
            }
        }
        else {
            this->timer = 0;
            if (abs(this->groundVel) < 390594)
                this->animator.SetAnimation(this->aniFrames, ANI_WALKING, false, 0);
            else if (abs(this->groundVel) > 655359)
                this->animator.SetAnimation(this->aniFrames, ANI_PEELOUT, false, 0);
            else
                this->animator.SetAnimation(this->aniFrames, ANI_RUNNING, false, 0);
        }

        if (this->skidding) {
            if (this->skidding == 16)
                sVars->sfxSkidding.Play();
            this->animator.SetAnimation(this->aniFrames, ANI_SKIDDING, false, 0);
            this->skidding--;
        }

        if (this->pushing == 2)
            this->animator.SetAnimation(this->aniFrames, ANI_PUSHING, false, 0);

        if (this->jumpPress) {
            DefaultJumpAction(this);
            this->state.Set(&Player::State_Air_Movement);
            sVars->sfxJump.Play();
        }
        else {
            if (this->up && !this->groundVel && this->flailing[1]) {
                this->state.Set(&Player::State_Looking_Up);
                this->animator.SetAnimation(this->aniFrames, ANI_LOOKINGUP, false, 0);
                this->timer = 0;
            }

            if (this->down) {
                if (!this->groundVel) {
                    if (this->flailing[1]) {
                        this->state.Set(&Player::State_Looking_Down);
                        this->animator.SetAnimation(this->aniFrames, ANI_LOOKINGDOWN, false, 0);
                        this->timer = 0;
                    }
                }
                else {
                    if (abs(this->groundVel) > 6554) {
                        this->frictionLoss = 0;
                        this->state.Set(&Player::State_Rolling);
                        this->animator.SetAnimation(this->aniFrames, ANI_JUMPING, false, 0);
                        sVars->sfxSpin.Play();
                    }
                }
            }
        }
    }
}

void Player::State_Air_Movement(void)
{
    SET_CURRENT_STATE();

    DefaultAirMovement(this);

    if (!this->onGround) {
        DefaultGravityTrue(this);

        if (this->animator.animationID == ANI_BOUNCING && this->velocity.y >= 0)
            this->animator.SetAnimation(this->aniFrames, ANI_WALKING, false, 0);
    }
    else {
        this->state.Set(&Player::State_Normal_Ground_Movement);
        DefaultGravityTrue(this);
    }
}

void Player::State_Rolling(void)
{
    SET_CURRENT_STATE();

    DefaultRollingMovement(this);

    if (!this->onGround) {
        this->state.Set(&Player::State_Air_Movement);
        DefaultGravityTrue(this);
    }
    else {
        DefaultGravityFalse(this);
        if (this->jumpPress) {
            DefaultJumpAction(this);
            this->state.Set(&Player::State_Rolling_Jump);
            sVars->sfxJump.Play();
        }
    }
}

void Player::State_Rolling_Jump(void)
{
    SET_CURRENT_STATE();

    this->left  = false;
    this->right = false;
    DefaultAirMovement(this);

    if (!this->onGround) {
        DefaultGravityTrue(this);
    }
    else {
        this->state.Set(&Player::State_Normal_Ground_Movement);
        DefaultGravityFalse(this);
    }
}

void Player::State_Looking_Up(void)
{
    SET_CURRENT_STATE();

    if (!this->up) {
        this->state.Set(&Player::State_Normal_Ground_Movement);
        this->timer = 0;
    }
    else {
        if (this->timer < 60)
            this->timer++;
        else if (this->lookPos > -112)
            this->lookPos -= 2;

        if (!this->onGround) {
            this->state.Set(&Player::State_Air_Movement);
            this->timer = 0;
        }
        else if (this->jumpPress) {
            this->state.Set(&Player::State_Peelout);
            this->spinDash = 0;
            sVars->sfxCharge.Play();
        }
    }
}

void Player::State_Looking_Down(void)
{
    SET_CURRENT_STATE();

    if (!this->down) {
        this->state.Set(&Player::State_Normal_Ground_Movement);
        this->timer = 0;
    }
    else {
        if (this->timer < 60)
            this->timer++;
        else if (this->lookPos < 96)
            this->lookPos += 2;

        if (!this->onGround) {
            this->state.Set(&Player::State_Air_Movement);
            this->timer = 0;
        }
        else if (this->jumpPress) {
            this->state.Set(&Player::State_Spindash);
            this->animator.SetAnimation(this->aniFrames, ANI_SPINDASH, false, 0);
            this->spinDash = 0;
            sVars->sfxCharge.Play();
        }
    }
}

void Player::State_Spindash(void)
{
    SET_CURRENT_STATE();

    if (!this->onGround) {
        this->state.Set(&Player::State_Air_Movement);
        DefaultGravityTrue(this);
    }
    else
        DefaultGravityFalse(this);

    if (this->jumpPress) {
        if (this->spinDash < 512)
            this->spinDash += 64;
        this->animator.frameID = 0;
        sVars->sfxCharge.Play();
    }
    else if (this->spinDash)
        this->spinDash--;

    if (!this->down) {
        this->state.Set(&Player::State_Rolling);
        this->animator.SetAnimation(this->aniFrames, ANI_JUMPING, false, 0);

        int32 dashSpeed = (this->spinDash << 9) + 524288;
        this->groundVel = !this->direction ? dashSpeed : -dashSpeed;
        DefaultGravityTrue(this);
        sVars->sfxRelease.Play();
    }
}

void Player::State_Peelout(void)
{
    SET_CURRENT_STATE();

    if (!this->onGround) {
        this->state.Set(&Player::State_Air_Movement);
        this->groundVel = 0;
    }

    if (this->spinDash < 786432)
        this->spinDash += 12288;

    int32 animSpeed = 0;

    if (this->spinDash < 390594) {
        this->animator.SetAnimation(this->aniFrames, ANI_WALKING, false, 0);
        animSpeed = ((this->spinDash >> 16) * this->walkingSpeed / 6) + 20;
    }
    else {
        this->animator.SetAnimation(this->aniFrames, this->spinDash > 655359 ? ANI_PEELOUT : ANI_RUNNING, false, 0);
        animSpeed = ((this->spinDash >> 16) * this->runningSpeed / 6) + 20;
    }

    if (!this->up) {
        DefaultGravityTrue(this);
        this->state.Set(&Player::State_Normal_Ground_Movement);
        this->groundVel = !this->direction ? this->spinDash : -this->spinDash;
        sVars->sfxRelease.Play();
    }

    this->animator.speed = animSpeed;
}

void Player::State_Getting_Hurt(void)
{
    SET_CURRENT_STATE();

    uint8 hurtType = HURT_NONE;

    if (GameObject::Find("BlueShield") && false) {
        hurtType = HURT_HASSHIELD;
        // shield->Destroy();
        sVars->sfxHurt.Play();
    }
    else if (!this->rings) {
        sVars->sfxHurt.Play();
        hurtType = HURT_DIE;
    }
    else {
        sVars->sfxLoseRings.Play();
        hurtType = HURT_RINGLOSS;
    }

    switch (hurtType) {
        case HURT_HASSHIELD: {
            // Ouch!
            this->state.Set(&Player::State_Hurt_Recoil);
            this->animator.SetAnimation(this->aniFrames, ANI_HURT, false, 0);
            this->velocity.y  = -262144;
            this->onGround    = false;
            this->trackScroll = true;

            this->invincibility = 8000;

            if (this->water) {
                this->groundVel >>= 1;
                this->velocity.y >>= 1;
            }
            break;
        }
        case HURT_RINGLOSS: {
            // Ouch!
            this->state.Set(&Player::State_Hurt_Recoil);
            this->animator.SetAnimation(this->aniFrames, ANI_HURT, false, 0);
            this->velocity.y  = -262144;
            this->onGround    = false;
            this->trackScroll = true;

            this->invincibility = 8000;

            if (this->water) {
                this->groundVel >>= 1;
                this->velocity.y >>= 1;
            }

            int32 ringPool1 = this->rings;
            int32 ringPool2 = 0;
            if (ringPool1 > 16) {
                ringPool2 = ringPool1 - 16;
                ringPool1 = 16;
            }

            if (ringPool2 > 16)
                ringPool2 = 16;

            int32 angleVal = (ringPool2 >> 1) << 5;
            int32 angle    = 384 - angleVal;

            if ((angleVal >> 4) == ringPool2)
                angle += 16;
            else
                angle -= 16;

            for (int32 r = 0; r < ringPool2; r++) {
                // Ring *ring = CREATE_ENTITY(Ring, this->position, this->position.x, this->position.y);
                // ring->velocity.x = Math::Cos256(angle) << 8;
                // ring->velocity.y = Math::Sin256(angle) << 8;

                angle += 32;
            }

            angleVal = (ringPool1 >> 1) << 5;
            angle    = 384 - angleVal;

            if ((angleVal >> 4) == ringPool1)
                angle += 16;
            else
                angle -= 16;

            for (int32 r = 0; r < ringPool1; r++) {
                // Ring *ring       = CREATE_ENTITY(Ring, this->position, this->position.x, this->position.y);
                // ring->velocity.x = Math::Cos256(angle) << 9;
                // ring->velocity.y = Math::Sin256(angle) << 9;

                angle += 32;
            }

            this->rings         = 0;
            this->ringExtraLife = 99;
            break;
        }
        case HURT_DIE: {
            // Gadzooks!!
            this->groundVel  = 0;
            this->velocity.y = -425984;
            this->state.Set(&Player::State_Dying);
            this->animator.SetAnimation(this->aniFrames, ANI_DYING, true, 0);
            this->tileCollisions  = false;
            this->interaction     = false;
            this->camera->enabled = false;
            break;
        }
    }
}

void Player::State_Hurt_Recoil(void)
{
    SET_CURRENT_STATE();

    if (!this->onGround)
        DefaultGravityTrue(this);
    else {
        this->state.Set(&Player::State_Normal_Ground_Movement);
        this->invincibility = 120;
        this->flashing      = 3;
        this->groundVel     = 0;
        this->velocity.x    = 0;
        DefaultGravityFalse(this);
    }
}

void Player::State_Dying(void)
{
    SET_CURRENT_STATE();

    this->velocity.y += 14336;
    this->position.y += this->velocity.y;

    if (this->velocity.y > 1048576) {
        if (globals->lives)
            globals->lives--;

        sceneInfo->timeEnabled = false;

        // GameObject::Reset(this->Slot(), DeathEvent::sVars->classID, false);
        Stage::LoadScene();
    }
}

void Player::State_Drowning(void)
{
    SET_CURRENT_STATE();

    // I'll do it Later
}

void Player::State_Hanging(void)
{
    SET_CURRENT_STATE();

    // Nothing happens when you're hanging
}

void Player::State_Corkscrew_Run(void)
{
    SET_CURRENT_STATE();

    this->angle = 0;
    DefaultGroundMovement(this);
    this->animator.SetAnimation(this->aniFrames, ANI_CORKSCREW, false, 0);

    if (abs(this->groundVel) < 393216) {
        this->animator.SetAnimation(this->aniFrames, ANI_WALKING, false, 0);
        this->state.Set(&Player::State_Air_Movement);
        this->rotation = 0;
        if (this->groundVel < 0)
            this->direction = FLIP_X;
    }

    if (this->down && abs(this->groundVel) > 6554) {
        this->state.Set(&Player::State_Corkscrew_Roll);
        this->animator.SetAnimation(this->aniFrames, ANI_JUMPING, false, 0);
        sVars->sfxSpin.Play();
    }

    if (this->skidding) {
        if (this->skidding == 16)
            sVars->sfxSkidding.Play();
        this->animator.SetAnimation(this->aniFrames, ANI_SKIDDING, false, 0);
        this->skidding--;
    }

    if (this->jumpPress) {
        DefaultJumpAction(this);
        this->state.Set(&Player::State_Air_Movement);
        sVars->sfxJump.Play();
    }
    else
        DefaultGravityFalse(this);
}

void Player::State_Corkscrew_Roll(void)
{
    SET_CURRENT_STATE();

    this->angle = 0;
    DefaultRollingMovement(this);

    if (abs(this->groundVel) < 393216)
        this->state.Set(&Player::State_Air_Movement);

    if (this->jumpPress) {
        DefaultJumpAction(this);
        this->state.Set(&Player::State_Rolling_Jump);
        sVars->sfxJump.Play();
    }
    else
        DefaultGravityFalse(this);
}

void Player::State_Tube_Rolling(void)
{
    SET_CURRENT_STATE();

    if (this->groundVel > 0) {
        if (this->groundVel < this->minRollSpeed)
            this->groundVel = this->minRollSpeed;
        if (this->groundVel > 1048576)
            this->groundVel = 1048576;
    }
    else {
        if (this->groundVel > -this->minRollSpeed) {
            this->groundVel = -this->minRollSpeed;
            DefaultGravityFalse(this);
        }
        if (this->groundVel < -1048576)
            this->groundVel = -1048576;
    }

    if (!this->onGround) {
        this->state.Set(&Player::State_Air_Movement);
        DefaultGravityTrue(this);
    }
    else {
        DefaultRollingMovement(this);
        DefaultGravityFalse(this);
    }
}

#if GAME_INCLUDE_EDITOR
void Player::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->sonicFrames, ANI_STOPPED, true, 0);
    this->animator.DrawSprite(NULL, false);
}

void Player::EditorLoad(void) { sVars->sonicFrames.Load("Players/SonicClassic.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void Player::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Player); }
#endif

void Player::Serialize(void) {}

} // namespace GameLogic
