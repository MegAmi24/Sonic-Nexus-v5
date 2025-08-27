// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: Bridge Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "Bridge.hpp"
#include "Math.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Bridge);

void Bridge::Update(void)
{
    foreach_active(Player, player)
    {
        if (!(player->position.x > this->left && player->position.x < this->right))
            continue;

        if (this->target == player) {
            this->midPoint = player->position.x - this->left;

            int32 midPoint = this->midPoint >> 8;
            int32 diff     = this->right - this->left;
            midPoint /= FROM_FIXED(diff);

            this->midHeight = Sin512(midPoint) * (diff >> 13);

            if (player->position.y > (this->position.y - 3145728) && player->velocity.y >= 0) {
                this->walkedOn++;
                player->position.y  = this->position.y + (TO_FIXED(-player->outerbox->bottom) + this->midYPos - 524288);
                player->onGround    = true;
                player->velocity.y  = 0;
                player->flailing[0] = true;
                player->flailing[1] = true;
                player->flailing[2] = true;
            }
        }
        else if (player->velocity.y >= 0) {
            int32 angle;

            int32 playerDiff = player->position.x - this->left;
            if (playerDiff > this->midPoint) {
                playerDiff     = this->right - player->position.x;
                int32 midPoint = this->right - this->left - this->midPoint;
                angle          = (playerDiff << 7) / midPoint;
            }
            else
                angle = (playerDiff << 7) / this->midPoint;

            int32 midYPos = ((Sin512(angle) * this->midYPos) >> 9) - 524288;

            Hitbox hitbox;
            hitbox.left  = -1024;
            hitbox.right = 1024;

            if (player->velocity.y < 32768) {
                hitbox.bottom = FROM_FIXED(midYPos);
                hitbox.top    = hitbox.bottom - 8;
            }
            else {
                hitbox.top    = FROM_FIXED(midYPos);
                hitbox.bottom = hitbox.top + 8;
            }

            if (this->CheckCollisionTouchBox(&hitbox, player, player->outerbox)) {
                this->walkedOn++;
                player->position.y = TO_FIXED(-player->outerbox->bottom) + this->position.y + midYPos;

                player->flailing[0] = true;
                player->flailing[1] = true;
                player->flailing[2] = true;

                if (player->onGround)
                    this->depress = 128;
                else
                    player->onGround = true;
                player->velocity.y = 0;

                if (player->Slot() == SLOT_PLAYER1)
                    this->target = player;
            }
        }
    }

    if (this->walkedOn) {
        if (this->depress < 128)
            this->depress += 8;
    }
    else if (this->depress) {
        this->depress -= 8;
        this->target = nullptr;
    }

    this->midYPos  = (this->midHeight * this->depress) >> 7;
    this->walkedOn = 0;
}

void Bridge::LateUpdate(void) {}
void Bridge::StaticUpdate(void) {}

void Bridge::Draw(void)
{
    int32 s = 0;
    Vector2 pos;
    pos.x    = this->left + 524288;
    int32 sc = 524288;
    while (s < (this->midPoint >> 20)) {
        pos.y = ((Sin512((sc << 7) / this->midPoint) * this->midYPos) >> 9) + this->position.y;
        this->animator.DrawSprite(&pos, false);
        pos.x += 1048576;
        sc += 1048576;
        s++;
    }

    pos.y = this->midYPos + this->position.y;
    this->animator.DrawSprite(&pos, false);
    pos.x += 1048576;
    s++;

    int32 midPoint = this->right - this->left - this->midPoint;

    pos.x = this->right - 524288;
    sc    = 524288;

    while (s < this->length) {
        pos.y = ((Sin512((sc << 7) / midPoint) * this->midYPos) >> 9) + this->position.y;
        this->animator.DrawSprite(&pos, false);
        pos.x -= 1048576;
        sc += 1048576;
        s++;
    }
}

void Bridge::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->length = VOID_TO_INT(data);
        this->animator.SetAnimation(sVars->aniFrames, 2, true, 0);
        this->active        = ACTIVE_BOUNDS;
        this->updateRange.x = TO_FIXED(128);
        this->updateRange.y = TO_FIXED(128);
        this->drawGroup     = 3;

        this->right = this->length << 19;
        this->left  = -this->right + this->position.x;
        this->right += this->position.x;
        this->target = nullptr;
    }
}

void Bridge::StageLoad(void) { sVars->aniFrames.Load("SSZ/Platform.bin", SCOPE_GLOBAL); }

#if GAME_INCLUDE_EDITOR
void Bridge::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 2, true, 0);

    Vector2 pos   = this->position;
    int32 xOffset = TO_FIXED(-(this->animator.GetFrame(sVars->aniFrames)->width * (this->length / 2)));
    if (this->length % 2 != 1)
        xOffset -= TO_FIXED(this->animator.GetFrame(sVars->aniFrames)->pivotX);
    pos.x += xOffset;
    for (int32 i = 0; i < this->length; i++) {
        this->animator.DrawSprite(&pos, false);
        pos.x += TO_FIXED(this->animator.GetFrame(sVars->aniFrames)->width);
    }
}

void Bridge::EditorLoad(void) { sVars->aniFrames.Load("SSZ/Platform.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void Bridge::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Bridge); }
#endif

void Bridge::Serialize(void) { RSDK_EDITABLE_VAR(Bridge, VAR_INT32, length); }

} // namespace GameLogic
