// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Camera Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Camera.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Camera);

void Camera::Update(void) {}

void Camera::LateUpdate(void)
{
    if (this->target->classID == $(Player)->classID) {
        if (this->enabled) {
            switch (this->style) {
                case CAMERASTYLE_FOLLOW: SetPlayerScreenPosition(this->target); break;
                case CAMERASTYLE_EXTENDED: SetPlayerScreenPositionCDStyle(this->target); break;
                default: break;
            }
        }
        else {
            SetPlayerLockedScreenPosition(this->target);
        }
    }

    ScreenInfo *screen = &screenInfo[this->target->Slot()];
    this->center.x     = this->scrollA.x + screen->center.x;
    this->center.y     = this->scrollA.y + screen->center.y;
    screen->position   = this->scrollA;
}

void Camera::StaticUpdate(void) {}
void Camera::Draw(void) {}

void Camera::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        int32 screen = data ? VOID_TO_INT(data) : SLOT_PLAYER1;
        this->target = RSDK_GET_ENTITY_GEN(screen);

        if (this->active != ACTIVE_NORMAL)
            Graphics::AddCamera(&this->center, TO_FIXED(screenInfo[screen].center.x), TO_FIXED(screenInfo[screen].center.y), false);

        this->active  = ACTIVE_NORMAL;
        this->enabled = true;
        this->style   = CAMERASTYLE_EXTENDED;

        this->scrollA.x = (this->target->position.x >> 16) - screenInfo[screen].center.x;
        this->scrollB.x = (this->target->position.x >> 16) - screenInfo[screen].center.x + screenInfo[screen].size.x;
        this->scrollA.y = (this->target->position.y >> 16) - SCREEN_SCROLL_UP;
        this->scrollB.y = (this->target->position.y >> 16) - SCREEN_SCROLL_UP + screenInfo[screen].size.y;

        this->center.x = this->scrollA.x + screenInfo[screen].center.x;
        this->center.y = this->scrollA.y + screenInfo[screen].center.y;
    }
}

void Camera::StageLoad(void)
{
    GameObject::Reset(SLOT_CAMERA1, sVars->classID, SLOT_PLAYER1);

    // Get Layer size and setup default bounds
    SceneLayer fgLayer;
    fgLayer.Get("FG Low");
    fgLayer.Size(&sVars->boundary2, true);

    sVars->boundary1    = { 0, 0 };
    sVars->waterLevel   = sVars->boundary2.y + 128;
    sVars->newBoundary1 = sVars->boundary1;
    sVars->newBoundary2 = sVars->boundary2;
}

void Camera::SetPlayerScreenPosition(Entity *entity)
{
    Player *player = (Player *)entity;

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
        if (this->scrollOffset.x <= sVars->boundary1.x)
            --sVars->boundary1.x;
        else
            sVars->boundary1.x = sVars->newBoundary1.x;
    }
    if (sVars->newBoundary2.x < sVars->boundary2.x) {
        if (this->scrollOffset.x + screenInfo->size.x >= sVars->boundary2.x)
            sVars->boundary2.x = this->scrollOffset.x + screenInfo->size.x;
        else
            sVars->boundary2.x = sVars->newBoundary2.x;
    }
    if (sVars->newBoundary2.x > sVars->boundary2.x) {
        if (this->scrollOffset.x + screenInfo->size.x >= sVars->boundary2.x)
            ++sVars->boundary2.x;
        else 
            sVars->boundary2.x = sVars->newBoundary2.x;
    }
    int32 xscrollA     = this->scrollA.x;
    int32 xscrollB     = this->scrollB.x;
    int32 scrollAmount = playerXPos - (this->scrollA.x + screenInfo->center.x);
    if (abs(scrollAmount) >= 25) {
        if (scrollAmount <= 0)
            xscrollA -= 16;
        else
            xscrollA += 16;
        xscrollB = xscrollA + screenInfo->size.x;
    }
    else {
        if (playerXPos > xscrollA + SCREEN_SCROLL_RIGHT) {
            xscrollA = playerXPos - SCREEN_SCROLL_RIGHT;
            xscrollB = playerXPos - SCREEN_SCROLL_RIGHT + screenInfo->size.x;
        }
        if (playerXPos < xscrollA + SCREEN_SCROLL_LEFT) {
            xscrollA = playerXPos - SCREEN_SCROLL_LEFT;
            xscrollB = playerXPos - SCREEN_SCROLL_LEFT + screenInfo->size.x;
        }
    }
    if (xscrollA < sVars->boundary1.x) {
        xscrollA = sVars->boundary1.x;
        xscrollB = sVars->boundary1.x + screenInfo->size.x;
    }
    if (xscrollB > sVars->boundary2.x) {
        xscrollB = sVars->boundary2.x;
        xscrollA = sVars->boundary2.x - screenInfo->size.x;
    }
    this->scrollA.x = xscrollA;
    this->scrollB.x = xscrollB;
    if (playerXPos <= xscrollA + screenInfo->center.x) {
        player->screenPos.x  = this->earthquake.x + playerXPos - xscrollA;
        this->scrollOffset.x = xscrollA - this->earthquake.x;
    }
    else {
        this->scrollOffset.x = playerXPos + this->earthquake.x - screenInfo->center.x;
        player->screenPos.x  = screenInfo->center.x - this->earthquake.x;
        if (playerXPos > xscrollB - screenInfo->center.x) {
            player->screenPos.x  = playerXPos - (xscrollB - screenInfo->center.x) + this->earthquake.x + screenInfo->center.x;
            this->scrollOffset.x = xscrollB - screenInfo->size.x - this->earthquake.x;
        }
    }
    int32 yscrollA     = this->scrollA.y;
    int32 yscrollB     = this->scrollB.y;
    int32 hitboxDiff   = player->normalbox->bottom - player->outerbox->bottom;
    int32 adjustYPos   = playerYPos - hitboxDiff;
    int32 adjustAmount = player->lookPos + adjustYPos - (this->scrollA.y + SCREEN_SCROLL_UP);
    if (player->trackScroll) {
        this->scrollMove.y = 32;
    }
    else {
        if (this->scrollMove.y == 32) {
            this->scrollMove.y = 2 * ((hitboxDiff + SCREEN_SCROLL_UP - player->screenPos.y - player->lookPos) >> 1);
            if (this->scrollMove.y > 32)
                this->scrollMove.y = 32;
            if (this->scrollMove.y < -32)
                this->scrollMove.y = -32;
        }
        if (this->scrollMove.y > 0)
            this->scrollMove.y -= 6;
        this->scrollMove.y += this->scrollMove.y < 0 ? 6 : 0;
    }
    int32 absAdjust = abs(adjustAmount);
    if (absAdjust >= abs(this->scrollMove.y) + 17) {
        if (adjustAmount <= 0)
            yscrollA -= 16;
        else
            yscrollA += 16;
        yscrollB = yscrollA + screenInfo->size.y;
    }
    else if (this->scrollMove.y == 32) {
        if (player->lookPos + adjustYPos > yscrollA + this->scrollMove.y + SCREEN_SCROLL_UP) {
            yscrollA = player->lookPos + adjustYPos - (this->scrollMove.y + SCREEN_SCROLL_UP);
            yscrollB = yscrollA + screenInfo->size.y;
        }
        if (player->lookPos + adjustYPos < yscrollA + SCREEN_SCROLL_UP - this->scrollMove.y) {
            yscrollA = player->lookPos + adjustYPos - (SCREEN_SCROLL_UP - this->scrollMove.y);
            yscrollB = yscrollA + screenInfo->size.y;
        }
    }
    else {
        yscrollA = player->lookPos + adjustYPos + this->scrollMove.y - SCREEN_SCROLL_UP;
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
        if (this->earthquake.y <= 0)
            this->earthquake.y = ~this->earthquake.y;
        else
            this->earthquake.y = -this->earthquake.y;
    }

    if (player->lookPos + adjustYPos <= yscrollA + SCREEN_SCROLL_UP) {
        player->screenPos.y  = adjustYPos - yscrollA - this->earthquake.y;
        this->scrollOffset.y = this->earthquake.y + this->scrollA.y;
    }
    else {
        this->scrollOffset.y = this->earthquake.y + adjustYPos + player->lookPos - SCREEN_SCROLL_UP;
        player->screenPos.y  = SCREEN_SCROLL_UP - player->lookPos - this->earthquake.y;
        if (player->lookPos + adjustYPos > yscrollB - SCREEN_SCROLL_DOWN) {
            player->screenPos.y  = adjustYPos - (yscrollB - SCREEN_SCROLL_DOWN) + this->earthquake.y + SCREEN_SCROLL_UP;
            this->scrollOffset.y = yscrollB - screenInfo->size.y - this->earthquake.y;
        }
    }
    player->screenPos.y += hitboxDiff;
}

void Camera::SetPlayerScreenPositionCDStyle(Entity *entity)
{
    Player *player = (Player *)entity;

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
        if (this->scrollOffset.x <= sVars->boundary1.x)
            --sVars->boundary1.x;
        else
            sVars->boundary1.x = sVars->newBoundary1.x;
    }
    if (sVars->newBoundary2.x < sVars->boundary2.x) {
        if (this->scrollOffset.x + screenInfo->size.x >= sVars->boundary2.x)
            sVars->boundary2.x = this->scrollOffset.x + screenInfo->size.x;
        else
            sVars->boundary2.x = sVars->newBoundary2.x;
    }
    if (sVars->newBoundary2.x > sVars->boundary2.x) {
        if (this->scrollOffset.x + screenInfo->size.x >= sVars->boundary2.x)
            ++sVars->boundary2.x;
        else
            sVars->boundary2.x = sVars->newBoundary2.x;
    }
    if (player->onGround) {
        if (player->direction) {
            if (player->animator.animationID == Player::ANI_PEELOUT || player->animator.animationID == Player::ANI_SPINDASH
                || player->groundVel < -0x5F5C2) {
                if (this->scrollMove.x < 64)
                    this->scrollMove.x += 2;
            }
            else {
                this->scrollMove.x += this->scrollMove.x < 0 ? 2 : 0;
                if (this->scrollMove.x > 0)
                    this->scrollMove.x -= 2;
            }
        }
        else if (player->animator.animationID == Player::ANI_PEELOUT || player->animator.animationID == Player::ANI_SPINDASH
                 || player->groundVel > 0x5F5C2) {
            if (this->scrollMove.x > -64)
                this->scrollMove.x -= 2;
        }
        else {
            this->scrollMove.x += this->scrollMove.x < 0 ? 2 : 0;
            if (this->scrollMove.x > 0)
                this->scrollMove.x -= 2;
        }
    }
    if (playerXPos <= sVars->boundary1.x + this->scrollMove.x + screenInfo->center.x) {
        player->screenPos.x  = this->earthquake.x + playerXPos - sVars->boundary1.x;
        this->scrollOffset.x = sVars->boundary1.x - this->earthquake.x;
    }
    else {
        this->scrollOffset.x = playerXPos + this->earthquake.x - screenInfo->center.x - this->scrollMove.x;
        player->screenPos.x  = this->scrollMove.x + screenInfo->center.x - this->earthquake.x;
        if (playerXPos - this->scrollMove.x > sVars->boundary2.x - screenInfo->center.x) {
            player->screenPos.x  = playerXPos - (sVars->boundary2.x - screenInfo->center.x) + this->earthquake.x + screenInfo->center.x;
            this->scrollOffset.x = sVars->boundary2.x - screenInfo->size.x - this->earthquake.x;
        }
    }
    this->scrollA.x    = this->scrollOffset.x;
    this->scrollB.x    = screenInfo->size.x + this->scrollOffset.x;
    int32 yscrollA     = this->scrollA.y;
    int32 yscrollB     = this->scrollB.y;
    int32 hitboxDiff   = player->normalbox->bottom - player->outerbox->bottom;
    int32 adjustY      = playerYPos - hitboxDiff;
    int32 adjustOffset = player->lookPos + adjustY - (this->scrollA.y + SCREEN_SCROLL_UP);
    if (player->trackScroll) {
        this->scrollMove.y = 32;
    }
    else {
        if (this->scrollMove.y == 32) {
            this->scrollMove.y = 2 * ((hitboxDiff + SCREEN_SCROLL_UP - player->screenPos.y - player->lookPos) >> 1);
            if (this->scrollMove.y > 32)
                this->scrollMove.y = 32;
            if (this->scrollMove.y < -32)
                this->scrollMove.y = -32;
        }
        if (this->scrollMove.y > 0)
            this->scrollMove.y -= 6;
        this->scrollMove.y += this->scrollMove.y < 0 ? 6 : 0;
    }
    int32 absAdjust = abs(adjustOffset);
    if (absAdjust >= abs(this->scrollMove.y) + 17) {
        if (adjustOffset <= 0)
            yscrollA -= 16;
        else
            yscrollA += 16;
        yscrollB = yscrollA + screenInfo->size.y;
    }
    else if (this->scrollMove.y == 32) {
        if (player->lookPos + adjustY > yscrollA + this->scrollMove.y + SCREEN_SCROLL_UP) {
            yscrollA = player->lookPos + adjustY - (this->scrollMove.y + SCREEN_SCROLL_UP);
            yscrollB = yscrollA + screenInfo->size.y;
        }
        if (player->lookPos + adjustY < yscrollA + SCREEN_SCROLL_UP - this->scrollMove.y) {
            yscrollA = player->lookPos + adjustY - (SCREEN_SCROLL_UP - this->scrollMove.y);
            yscrollB = yscrollA + screenInfo->size.y;
        }
    }
    else {
        yscrollA = player->lookPos + adjustY + this->scrollMove.y - SCREEN_SCROLL_UP;
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
        if (this->earthquake.y <= 0)
            this->earthquake.y = ~this->earthquake.y;
        else
            this->earthquake.y = -this->earthquake.y;
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
    player->screenPos.y += hitboxDiff;
}

void Camera::SetPlayerLockedScreenPosition(Entity *entity)
{
    Player *player = (Player *)entity;

    int32 playerXPos = player->position.x >> 16;
    int32 playerYPos = player->position.y >> 16;
    switch (this->style) {
        case CAMERASTYLE_FOLLOW:
        {
            if (playerXPos <= sVars->boundary1.x + this->scrollMove.x + screenInfo->center.x) {
                player->screenPos.x = this->earthquake.x + playerXPos - sVars->boundary1.x;
                this->scrollOffset.x = sVars->boundary1.x - this->earthquake.x;
            }
            else {
                this->scrollOffset.x = playerXPos + this->earthquake.x - screenInfo->center.x - this->scrollMove.x;
                player->screenPos.x = this->scrollMove.x + screenInfo->center.x - this->earthquake.x;
                if (playerXPos > sVars->boundary2.x + this->scrollMove.x - screenInfo->center.x) {
                    player->screenPos.x = this->scrollMove.x + playerXPos - (sVars->boundary2.x - screenInfo->center.x) + this->earthquake.x + screenInfo->center.x;
                    this->scrollOffset.x = sVars->boundary2.x - screenInfo->size.x - this->earthquake.x - this->scrollMove.x;
                }
            }
            break;
        }
        case CAMERASTYLE_EXTENDED:
        {
            int32 xscrollA = this->scrollA.x;
            int32 xscrollB = this->scrollB.x;
            if (playerXPos <= this->scrollA.x + screenInfo->center.x) {
                player->screenPos.x = this->earthquake.x + playerXPos - this->scrollA.x;
                this->scrollOffset.x = xscrollA - this->earthquake.x;
            }
            else {
                this->scrollOffset.x = playerXPos + this->earthquake.x - screenInfo->center.x;
                player->screenPos.x = screenInfo->center.x - this->earthquake.x;
                if (playerXPos > xscrollB - screenInfo->center.x) {
                    player->screenPos.x = playerXPos - (xscrollB - screenInfo->center.x) + this->earthquake.x + screenInfo->center.x;
                    this->scrollOffset.x = xscrollB - screenInfo->size.x - this->earthquake.x;
                }
            }
            break;
        }
        default: break;
    }
    int32 yscrollA   = this->scrollA.y;
    int32 yscrollB   = this->scrollB.y;
    int32 hitboxDiff = player->normalbox->bottom - player->outerbox->bottom;
    int32 adjustY    = playerYPos - hitboxDiff;

    if (this->earthquake.y) {
        if (this->earthquake.y <= 0)
            this->earthquake.y = ~this->earthquake.y;
        else
            this->earthquake.y = -this->earthquake.y;
    }

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
    player->screenPos.y += hitboxDiff;
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
