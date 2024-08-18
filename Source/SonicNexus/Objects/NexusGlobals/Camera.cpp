// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Camera Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "Camera.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Camera);

void Camera::Update(void) {}

void Camera::LateUpdate(void)
{
    if (this->target->classID == Player::sVars->classID) {
        if (this->enabled) {
            switch (this->style) {
                case CAMERASTYLE_NORMAL: SetPlayerScreenPosition((Player *)this->target); break;
                case CAMERASTYLE_CDSTYLE: SetPlayerScreenPositionCDStyle((Player *)this->target); break;
                // HLocked is unused LOL
                default: break;
            }
        }
        else {
            SetPlayerLockedScreenPosition((Player *)this->target);
        }
    }

    ScreenInfo *screen = &screenInfo[this->target->Slot()];
    screen->position   = this->scrollA;
}

void Camera::StaticUpdate(void) {}

void Camera::Draw(void) {}

void Camera::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        int32 screen = VOID_TO_INT(data);
        this->target = RSDK_GET_ENTITY_GEN(screen);

        if (this->active != ACTIVE_NORMAL)
            Graphics::AddCamera(&this->target->position, TO_FIXED(screenInfo[screen].center.x), TO_FIXED(screenInfo[screen].center.y), true);

        this->active  = ACTIVE_NORMAL;
        this->enabled = true;
        this->style   = CAMERASTYLE_CDSTYLE;

        this->scrollA.x = (this->target->position.x >> 16) - screenInfo[screen].center.x;
        this->scrollB.x = (this->target->position.x >> 16) - screenInfo[screen].center.x + screenInfo[screen].size.x;
        this->scrollA.y = (this->target->position.y >> 16) - SCREEN_SCROLL_UP;
        this->scrollB.y = (this->target->position.y >> 16) - SCREEN_SCROLL_UP + screenInfo[screen].size.y;
    }
}

void Camera::StageLoad(void)
{
    GameObject::Reset(SLOT_CAMERA1, sVars->classID, SLOT_PLAYER1);

    // Get Layer size and setup default bounds
    RSDK::SceneLayer fgLayer;
    fgLayer.Get("FG Low");
    RSDK::Vector2 layerSize;
    fgLayer.Size(&layerSize, true);

    sVars->boundary2.x  = layerSize.x;
    sVars->boundary2.y  = layerSize.y;
    sVars->waterLevel   = sVars->boundary2.y + 128;
    sVars->newBoundary2 = sVars->boundary2;
}

void Camera::SetPlayerScreenPosition(Player *player)
{
    int32 playerXPos = player->position.x >> 16;
    int32 playerYPos = player->position.y >> 16;
    if (sVars->newBoundary1.y > sVars->boundary1.y) {
        if (this->scrollOffset.y <= sVars->newBoundary1.y)
            sVars->boundary1.y = this->scrollOffset.y;
        else
            sVars->boundary1.y = sVars->newBoundary1.y;
    }
    if (sVars->newBoundary1.y < sVars->boundary1.y) {
        if (this->scrollOffset.y <= sVars->boundary1.y)
            --sVars->boundary1.y;
        else
            sVars->boundary1.y = sVars->newBoundary1.y;
    }
    if (sVars->newBoundary2.y < sVars->boundary2.y) {
        if (this->scrollOffset.y + screenInfo->size.y >= sVars->boundary2.y || this->scrollOffset.y + screenInfo->size.y <= sVars->newBoundary2.y)
            --sVars->boundary2.y;
        else
            sVars->boundary2.y = this->scrollOffset.y + screenInfo->size.y;
    }
    if (sVars->newBoundary2.y > sVars->boundary2.y) {
        if (this->scrollOffset.y + screenInfo->size.y >= sVars->boundary2.y)
            ++sVars->boundary2.y;
        else
            sVars->boundary2.y = sVars->newBoundary2.y;
    }
    if (sVars->newBoundary1.x > sVars->boundary1.x) {
        if (this->scrollOffset.x <= sVars->newBoundary1.x)
            sVars->boundary1.x = this->scrollOffset.x;
        else
            sVars->boundary1.x = sVars->newBoundary1.x;
    }
    if (sVars->newBoundary1.x < sVars->boundary1.x) {
        if (this->scrollOffset.x <= sVars->boundary1.x) {
            --sVars->boundary1.x;
            if (player->velocity.x < 0) {
                sVars->boundary1.x += player->velocity.x >> 16;
                if (sVars->boundary1.x < sVars->newBoundary1.x)
                    sVars->boundary1.x = sVars->newBoundary1.x;
            }
        }
        else {
            sVars->boundary1.x = sVars->newBoundary1.x;
        }
    }
    if (sVars->newBoundary2.x < sVars->boundary2.x) {
        if (screenInfo->size.x + this->scrollOffset.x >= sVars->boundary2.x)
            sVars->boundary2.x = screenInfo->size.x + this->scrollOffset.x;
        else
            sVars->boundary2.x = sVars->newBoundary2.x;
    }
    if (sVars->newBoundary2.x > sVars->boundary2.x) {
        if (screenInfo->size.x + this->scrollOffset.x >= sVars->boundary2.x) {
            ++sVars->boundary2.x;
            if (player->velocity.x > 0) {
                sVars->boundary2.x += player->velocity.x >> 16;
                if (sVars->boundary2.x > sVars->newBoundary2.x)
                    sVars->boundary2.x = sVars->newBoundary2.x;
            }
        }
        else {
            sVars->boundary2.x = sVars->newBoundary2.x;
        }
    }
    int32 xscrollA     = this->scrollA.x;
    int32 xscrollB     = this->scrollB.x;
    int32 scrollAmount = playerXPos - (screenInfo->center.x + this->scrollA.x);
    if (abs(playerXPos - (screenInfo->center.x + this->scrollA.x)) >= 25) {
        if (scrollAmount <= 0)
            xscrollA -= 16;
        else
            xscrollA += 16;
        xscrollB = screenInfo->size.x + xscrollA;
    }
    else {
        if (playerXPos > SCREEN_SCROLL_RIGHT + xscrollA) {
            xscrollA = playerXPos - SCREEN_SCROLL_RIGHT;
            xscrollB = screenInfo->size.x + playerXPos - SCREEN_SCROLL_RIGHT;
        }
        if (playerXPos < SCREEN_SCROLL_LEFT + xscrollA) {
            xscrollA = playerXPos - SCREEN_SCROLL_LEFT;
            xscrollB = screenInfo->size.x + playerXPos - SCREEN_SCROLL_LEFT;
        }
    }
    if (xscrollA < sVars->boundary1.x) {
        xscrollA = sVars->boundary1.x;
        xscrollB = screenInfo->size.x + sVars->boundary1.x;
    }
    if (xscrollB > sVars->boundary2.x) {
        xscrollB = sVars->boundary2.x;
        xscrollA = sVars->boundary2.x - screenInfo->size.x;
    }

    this->scrollA.x = xscrollA;
    this->scrollB.x = xscrollB;
    if (playerXPos <= screenInfo->center.x + xscrollA) {
        player->screenPos.x  = this->earthquake.x + playerXPos - xscrollA;
        this->scrollOffset.x = xscrollA - this->earthquake.x;
    }
    else {
        this->scrollOffset.x = this->earthquake.x + playerXPos - screenInfo->center.x;
        player->screenPos.x  = screenInfo->center.x - this->earthquake.x;
        if (playerXPos > xscrollB - screenInfo->center.x) {
            player->screenPos.x  = this->earthquake.x + screenInfo->center.x + playerXPos - (xscrollB - screenInfo->center.x);
            this->scrollOffset.x = xscrollB - screenInfo->size.x - this->earthquake.x;
        }
    }

    int32 yscrollA     = this->scrollA.y;
    int32 yscrollB     = this->scrollB.y;
    int32 adjustYPos   = this->adjustY + playerYPos;
    int32 adjustAmount = player->lookPos + adjustYPos - (yscrollA + SCREEN_SCROLL_UP);
    if (player->trackScroll) {
        yScrollMove = 32;
    }
    else {
        if (yScrollMove == 32) {
            yScrollMove = 2 * ((SCREEN_SCROLL_UP - player->screenPos.y - player->lookPos) >> 1);
            if (yScrollMove > 32)
                yScrollMove = 32;
            if (yScrollMove < -32)
                yScrollMove = -32;
        }
        if (yScrollMove > 0)
            yScrollMove -= 6;
        yScrollMove += yScrollMove < 0 ? 6 : 0;
    }

    if (abs(adjustAmount) >= abs(yScrollMove) + 17) {
        if (adjustAmount <= 0)
            yscrollA -= 16;
        else
            yscrollA += 16;
        yscrollB = yscrollA + screenInfo->size.y;
    }
    else if (yScrollMove == 32) {
        if (player->lookPos + adjustYPos > yscrollA + yScrollMove + SCREEN_SCROLL_UP) {
            yscrollA = player->lookPos + adjustYPos - (yScrollMove + SCREEN_SCROLL_UP);
            yscrollB = yscrollA + screenInfo->size.y;
        }
        if (player->lookPos + adjustYPos < yscrollA + SCREEN_SCROLL_UP - yScrollMove) {
            yscrollA = player->lookPos + adjustYPos - (SCREEN_SCROLL_UP - yScrollMove);
            yscrollB = yscrollA + screenInfo->size.y;
        }
    }
    else {
        yscrollA = player->lookPos + adjustYPos + yScrollMove - SCREEN_SCROLL_UP;
        yscrollB = yscrollA + screenInfo->size.y;
    }
    if (yscrollA < sVars->boundary1.y) {
        yscrollA = sVars->boundary1.y;
        yscrollB = sVars->boundary1.y + screenInfo->size.y;
    }
    if (yscrollB > sVars->boundary2.y) {
        yscrollB = sVars->boundary2.y;
        yscrollA = sVars->boundary2.y - screenInfo->size.y;
    }
    this->scrollA.y = yscrollA;
    this->scrollB.y = yscrollB;
    if (player->lookPos + adjustYPos <= this->scrollA.y + SCREEN_SCROLL_UP) {
        player->screenPos.y  = adjustYPos - this->scrollA.y - this->earthquake.y;
        this->scrollOffset.y = this->earthquake.y + this->scrollA.y;
    }
    else {
        this->scrollOffset.y = this->earthquake.y + adjustYPos + player->lookPos - SCREEN_SCROLL_UP;
        player->screenPos.y  = SCREEN_SCROLL_UP - player->lookPos - this->earthquake.y;
        if (player->lookPos + adjustYPos > this->scrollB.y - SCREEN_SCROLL_DOWN) {
            player->screenPos.y  = adjustYPos - (this->scrollB.y - SCREEN_SCROLL_DOWN) + this->earthquake.y + SCREEN_SCROLL_UP;
            this->scrollOffset.y = this->scrollB.y - screenInfo->size.y - this->earthquake.y;
        }
    }
    player->screenPos.y -= this->adjustY;

    if (this->earthquake.x) {
        if (this->earthquake.x <= 0) {
            this->earthquake.x = ~this->earthquake.x;
        }
        else {
            this->earthquake.x = -this->earthquake.x;
        }
    }

    if (this->earthquake.y) {
        if (this->earthquake.y <= 0) {
            this->earthquake.y = ~this->earthquake.y;
        }
        else {
            this->earthquake.y = -this->earthquake.y;
        }
    }
}

void Camera::SetPlayerScreenPositionCDStyle(Player *player)
{
    int32 playerXPos = player->position.x >> 16;
    int32 playerYPos = player->position.y >> 16;
    if (sVars->newBoundary1.y > sVars->boundary1.y) {
        if (this->scrollOffset.y <= sVars->newBoundary1.y)
            sVars->boundary1.y = this->scrollOffset.y;
        else
            sVars->boundary1.y = sVars->newBoundary1.y;
    }
    if (sVars->newBoundary1.y < sVars->boundary1.y) {
        if (this->scrollOffset.y <= sVars->boundary1.y)
            --sVars->boundary1.y;
        else
            sVars->boundary1.y = sVars->newBoundary1.y;
    }
    if (sVars->newBoundary2.y < sVars->boundary2.y) {
        if (this->scrollOffset.y + screenInfo->size.y >= sVars->boundary2.y || this->scrollOffset.y + screenInfo->size.y <= sVars->newBoundary2.y)
            --sVars->boundary2.y;
        else
            sVars->boundary2.y = this->scrollOffset.y + screenInfo->size.y;
    }
    if (sVars->newBoundary2.y > sVars->boundary2.y) {
        if (this->scrollOffset.y + screenInfo->size.y >= sVars->boundary2.y)
            ++sVars->boundary2.y;
        else
            sVars->boundary2.y = sVars->newBoundary2.y;
    }
    if (sVars->newBoundary1.x > sVars->boundary1.x) {
        if (this->scrollOffset.x <= sVars->newBoundary1.x)
            sVars->boundary1.x = this->scrollOffset.x;
        else
            sVars->boundary1.x = sVars->newBoundary1.x;
    }
    if (sVars->newBoundary1.x < sVars->boundary1.x) {
        if (this->scrollOffset.x <= sVars->boundary1.x) {
            --sVars->boundary1.x;
            if (player->velocity.x < 0) {
                sVars->boundary1.x += player->velocity.x >> 16;
                if (sVars->boundary1.x < sVars->newBoundary1.x)
                    sVars->boundary1.x = sVars->newBoundary1.x;
            }
        }
        else {
            sVars->boundary1.x = sVars->newBoundary1.x;
        }
    }
    if (sVars->newBoundary2.x < sVars->boundary2.x) {
        if (screenInfo->size.x + this->scrollOffset.x >= sVars->boundary2.x)
            sVars->boundary2.x = screenInfo->size.x + this->scrollOffset.x;
        else
            sVars->boundary2.x = sVars->newBoundary2.x;
    }
    if (sVars->newBoundary2.x > sVars->boundary2.x) {
        if (screenInfo->size.x + this->scrollOffset.x >= sVars->boundary2.x) {
            ++sVars->boundary2.x;
            if (player->velocity.x > 0) {
                sVars->boundary2.x += player->velocity.x >> 16;
                if (sVars->boundary2.x > sVars->newBoundary2.x)
                    sVars->boundary2.x = sVars->newBoundary2.x;
            }
        }
        else {
            sVars->boundary2.x = sVars->newBoundary2.x;
        }
    }

    if (player->onGround) {
        if (player->direction) {
            if (player->animator.animationID == Player::ANI_PEELOUT || player->animator.animationID == Player::ANI_SPINDASH
                || player->groundVel < -0x5F5C2) {
                if (this->lag < 64)
                    this->lag += 2;
            }
            else {
                this->lag += this->lag < 0 ? 2 : 0;
                if (this->lag > 0)
                    this->lag -= 2;
            }
        }
        else if (player->animator.animationID == Player::ANI_PEELOUT || player->animator.animationID == Player::ANI_SPINDASH
                 || player->groundVel > 0x5F5C2) {
            if (this->lag > -64)
                this->lag -= 2;
        }
        else {
            this->lag += this->lag < 0 ? 2 : 0;
            if (this->lag > 0)
                this->lag -= 2;
        }
    }

    if (playerXPos <= this->lag + screenInfo->center.x + sVars->boundary1.x) {
        player->screenPos.x  = this->earthquake.x + playerXPos - sVars->boundary1.x;
        this->scrollOffset.x = sVars->boundary1.x - this->earthquake.x;
    }
    else {
        this->scrollOffset.x = this->earthquake.x + playerXPos - screenInfo->center.x - this->lag;
        player->screenPos.x  = this->lag + screenInfo->center.x - this->earthquake.x;
        if (playerXPos - this->lag > sVars->boundary2.x - screenInfo->center.x) {
            player->screenPos.x  = this->earthquake.x + screenInfo->center.x + playerXPos - (sVars->boundary2.x - screenInfo->center.x);
            this->scrollOffset.x = sVars->boundary2.x - screenInfo->size.x - this->earthquake.x;
        }
    }
    this->scrollA.x    = this->scrollOffset.x;
    this->scrollB.x    = screenInfo->size.x + this->scrollOffset.x;
    int32 yscrollA     = this->scrollA.y;
    int32 yscrollB     = this->scrollB.y;
    int32 adjustY      = this->adjustY + playerYPos;
    int32 adjustOffset = player->lookPos + adjustY - (this->scrollA.y + SCREEN_SCROLL_UP);
    if (player->trackScroll == 1) {
        yScrollMove = 32;
    }
    else {
        if (yScrollMove == 32) {
            yScrollMove = 2 * ((SCREEN_SCROLL_UP - player->screenPos.y - player->lookPos) >> 1);
            if (yScrollMove > 32)
                yScrollMove = 32;
            if (yScrollMove < -32)
                yScrollMove = -32;
        }
        if (yScrollMove > 0)
            yScrollMove -= 6;
        yScrollMove += yScrollMove < 0 ? 6 : 0;
    }

    int32 absAdjust = abs(adjustOffset);
    if (absAdjust >= abs(yScrollMove) + 17) {
        if (adjustOffset <= 0)
            yscrollA -= 16;
        else
            yscrollA += 16;
        yscrollB = yscrollA + screenInfo->size.y;
    }
    else if (yScrollMove == 32) {
        if (player->lookPos + adjustY > yscrollA + yScrollMove + SCREEN_SCROLL_UP) {
            yscrollA = player->lookPos + adjustY - (yScrollMove + SCREEN_SCROLL_UP);
            yscrollB = yscrollA + screenInfo->size.y;
        }
        if (player->lookPos + adjustY < yscrollA + SCREEN_SCROLL_UP - yScrollMove) {
            yscrollA = player->lookPos + adjustY - (SCREEN_SCROLL_UP - yScrollMove);
            yscrollB = yscrollA + screenInfo->size.y;
        }
    }
    else {
        yscrollA = player->lookPos + adjustY + yScrollMove - SCREEN_SCROLL_UP;
        yscrollB = yscrollA + screenInfo->size.y;
    }
    if (yscrollA < sVars->boundary1.y) {
        yscrollA = sVars->boundary1.y;
        yscrollB = sVars->boundary1.y + screenInfo->size.y;
    }
    if (yscrollB > sVars->boundary2.y) {
        yscrollB = sVars->boundary2.y;
        yscrollA = sVars->boundary2.y - screenInfo->size.y;
    }
    this->scrollA.y = yscrollA;
    this->scrollB.y = yscrollB;
    if (player->lookPos + adjustY <= yscrollA + SCREEN_SCROLL_UP) {
        player->screenPos.y  = adjustY - yscrollA - this->earthquake.y;
        this->scrollOffset.y = this->earthquake.y + yscrollA;
    }
    else {
        this->scrollOffset.y = this->earthquake.y + adjustY + player->lookPos - SCREEN_SCROLL_UP;
        player->screenPos.y  = SCREEN_SCROLL_UP - player->lookPos - this->earthquake.y;
        if (player->lookPos + adjustY > yscrollB - SCREEN_SCROLL_DOWN) {
            player->screenPos.y  = adjustY - (yscrollB - SCREEN_SCROLL_DOWN) + this->earthquake.y + SCREEN_SCROLL_UP;
            this->scrollOffset.y = yscrollB - screenInfo->size.y - this->earthquake.y;
        }
    }
    player->screenPos.y -= this->adjustY;

    if (this->earthquake.x) {
        if (this->earthquake.x <= 0) {
            this->earthquake.x = ~this->earthquake.x;
        }
        else {
            this->earthquake.x = -this->earthquake.x;
        }
    }

    if (this->earthquake.y) {
        if (this->earthquake.y <= 0) {
            this->earthquake.y = ~this->earthquake.y;
        }
        else {
            this->earthquake.y = -this->earthquake.y;
        }
    }
}

void Camera::SetPlayerHLockedScreenPosition(Player *player)
{
    int32 playerXPos = player->position.x >> 16;
    int32 playerYPos = player->position.y >> 16;
    if (sVars->newBoundary1.y > sVars->boundary1.y) {
        if (this->scrollOffset.y <= sVars->newBoundary1.y)
            sVars->boundary1.y = this->scrollOffset.y;
        else
            sVars->boundary1.y = sVars->newBoundary1.y;
    }
    if (sVars->newBoundary1.y < sVars->boundary1.y) {
        if (this->scrollOffset.y <= sVars->boundary1.y)
            --sVars->boundary1.y;
        else
            sVars->boundary1.y = sVars->newBoundary1.y;
    }
    if (sVars->newBoundary2.y < sVars->boundary2.y) {
        if (this->scrollOffset.y + screenInfo->size.y >= sVars->boundary2.y || this->scrollOffset.y + screenInfo->size.y <= sVars->newBoundary2.y)
            --sVars->boundary2.y;
        else
            sVars->boundary2.y = this->scrollOffset.y + screenInfo->size.y;
    }
    if (sVars->newBoundary2.y > sVars->boundary2.y) {
        if (this->scrollOffset.y + screenInfo->size.y >= sVars->boundary2.y)
            ++sVars->boundary2.y;
        else
            sVars->boundary2.y = sVars->newBoundary2.y;
    }

    int32 xscrollA = this->scrollA.x;
    int32 xscrollB = this->scrollB.x;
    if (playerXPos <= screenInfo->center.x + this->scrollA.x) {
        player->screenPos.x  = this->earthquake.x + playerXPos - this->scrollA.x;
        this->scrollOffset.x = xscrollA - this->earthquake.x;
    }
    else {
        this->scrollOffset.x = this->earthquake.x + playerXPos - screenInfo->center.x;
        player->screenPos.x  = screenInfo->center.x - this->earthquake.x;
        if (playerXPos > xscrollB - screenInfo->center.x) {
            player->screenPos.x  = this->earthquake.x + screenInfo->center.x + playerXPos - (xscrollB - screenInfo->center.x);
            this->scrollOffset.x = xscrollB - screenInfo->size.x - this->earthquake.x;
        }
    }

    int32 yscrollA   = this->scrollA.y;
    int32 yscrollB   = this->scrollB.y;
    int32 adjustY    = this->adjustY + playerYPos;
    int32 lookOffset = player->lookPos + adjustY - (this->scrollA.y + SCREEN_SCROLL_UP);
    if (player->trackScroll == 1) {
        yScrollMove = 32;
    }
    else {
        if (yScrollMove == 32) {
            yScrollMove = 2 * ((SCREEN_SCROLL_UP - player->screenPos.y - player->lookPos) >> 1);
            if (yScrollMove > 32)
                yScrollMove = 32;
            if (yScrollMove < -32)
                yScrollMove = -32;
        }
        if (yScrollMove > 0)
            yScrollMove -= 6;
        yScrollMove += yScrollMove < 0 ? 6 : 0;
    }

    int32 absLook = abs(lookOffset);
    if (absLook >= abs(yScrollMove) + 17) {
        if (lookOffset <= 0)
            yscrollA -= 16;
        else
            yscrollA += 16;
        yscrollB = yscrollA + screenInfo->size.y;
    }
    else if (yScrollMove == 32) {
        if (player->lookPos + adjustY > yscrollA + yScrollMove + SCREEN_SCROLL_UP) {
            yscrollA = player->lookPos + adjustY - (yScrollMove + SCREEN_SCROLL_UP);
            yscrollB = yscrollA + screenInfo->size.y;
        }
        if (player->lookPos + adjustY < yscrollA + SCREEN_SCROLL_UP - yScrollMove) {
            yscrollA = player->lookPos + adjustY - (SCREEN_SCROLL_UP - yScrollMove);
            yscrollB = yscrollA + screenInfo->size.y;
        }
    }
    else {
        yscrollA = player->lookPos + adjustY + yScrollMove - SCREEN_SCROLL_UP;
        yscrollB = yscrollA + screenInfo->size.y;
    }
    if (yscrollA < sVars->boundary1.y) {
        yscrollA = sVars->boundary1.y;
        yscrollB = sVars->boundary1.y + screenInfo->size.y;
    }
    if (yscrollB > sVars->boundary2.y) {
        yscrollB = sVars->boundary2.y;
        yscrollA = sVars->boundary2.y - screenInfo->size.y;
    }
    this->scrollA.y = yscrollA;
    this->scrollB.y = yscrollB;

    if (this->earthquake.y) {
        if (this->earthquake.y <= 0) {
            this->earthquake.y = -this->earthquake.y;
            --this->earthquake.y;
        }
        else {
            this->earthquake.y = -this->earthquake.y;
        }
    }

    if (player->lookPos + adjustY <= yscrollA + SCREEN_SCROLL_UP) {
        player->screenPos.y  = adjustY - yscrollA - this->earthquake.y;
        this->scrollOffset.y = this->earthquake.y + yscrollA;
    }
    else {
        this->scrollOffset.y = this->earthquake.y + adjustY + player->lookPos - SCREEN_SCROLL_UP;
        player->screenPos.y  = SCREEN_SCROLL_UP - player->lookPos - this->earthquake.y;
        if (player->lookPos + adjustY > yscrollB - SCREEN_SCROLL_DOWN) {
            player->screenPos.y  = adjustY - (yscrollB - SCREEN_SCROLL_DOWN) + this->earthquake.y + SCREEN_SCROLL_UP;
            this->scrollOffset.y = yscrollB - screenInfo->size.y - this->earthquake.y;
        }
    }
    player->screenPos.y -= this->adjustY;
}

void Camera::SetPlayerLockedScreenPosition(Player *player)
{
    int32 playerXPos = player->position.x >> 16;
    int32 playerYPos = player->position.y >> 16;
    int32 xscrollA   = this->scrollA.x;
    int32 xscrollB   = this->scrollB.x;
    if (playerXPos <= screenInfo->center.x + this->scrollA.x) {
        player->screenPos.x  = this->earthquake.x + playerXPos - this->scrollA.x;
        this->scrollOffset.x = xscrollA - this->earthquake.x;
    }
    else {
        this->scrollOffset.x = this->earthquake.x + playerXPos - screenInfo->center.x;
        player->screenPos.x  = screenInfo->center.x - this->earthquake.x;
        if (playerXPos > xscrollB - screenInfo->center.x) {
            player->screenPos.x  = this->earthquake.x + screenInfo->center.x + playerXPos - (xscrollB - screenInfo->center.x);
            this->scrollOffset.x = xscrollB - screenInfo->size.x - this->earthquake.x;
        }
    }

    int32 yscrollA = this->scrollA.y;
    int32 yscrollB = this->scrollB.y;
    int32 adjustY  = this->adjustY + playerYPos;
    if (player->lookPos + adjustY <= this->scrollA.y + SCREEN_SCROLL_UP) {
        player->screenPos.y  = adjustY - this->scrollA.y - this->earthquake.y;
        this->scrollOffset.y = this->earthquake.y + yscrollA;
    }
    else {
        this->scrollOffset.y = this->earthquake.y + adjustY + player->lookPos - SCREEN_SCROLL_UP;
        player->screenPos.y  = SCREEN_SCROLL_UP - player->lookPos - this->earthquake.y;
        if (player->lookPos + adjustY > yscrollB - SCREEN_SCROLL_DOWN) {
            player->screenPos.y  = adjustY - (yscrollB - SCREEN_SCROLL_DOWN) + this->earthquake.y + SCREEN_SCROLL_UP;
            this->scrollOffset.y = yscrollB - screenInfo->size.y - this->earthquake.y;
        }
    }
    player->screenPos.y -= this->adjustY;

    if (this->earthquake.x) {
        if (this->earthquake.x <= 0) {
            this->earthquake.x = ~this->earthquake.x;
        }
        else {
            this->earthquake.x = -this->earthquake.x;
        }
    }

    if (this->earthquake.y) {
        if (this->earthquake.y <= 0) {
            this->earthquake.y = ~this->earthquake.y;
        }
        else {
            this->earthquake.y = -this->earthquake.y;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Camera::EditorDraw(void) {}

void Camera::EditorLoad(void) {}
#endif

#if RETRO_REV0U
void Camera::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Camera); }
#endif

void Camera::Serialize(void) {}

} // namespace GameLogic
