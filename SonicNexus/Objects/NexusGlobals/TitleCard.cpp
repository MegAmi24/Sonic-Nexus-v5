// ---------------------------------------------------------------------
// RSDK Project: Sonic Nexus v5
// Object Description: TitleCard Object
// Original Author: Christian Whitehead "The Taxman"
// Ported to RSDKv5 By: MegAmi
// ---------------------------------------------------------------------

#include "TitleCard.hpp"
#include "Music.hpp"
#include "HUD.hpp"
#include "Players/Player.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(TitleCard);

void TitleCard::Update(void) { this->state.Run(this); }
void TitleCard::LateUpdate(void) { foreach_active(Player, player) player->timer = 0; }
void TitleCard::StaticUpdate(void) {}

void TitleCard::Draw(void)
{
    if (!this->stateDraw.Matches(StateMachine_None))
        this->stateDraw.Run(this);
    else
        this->animator.DrawSprite(NULL, true);
}

void TitleCard::Create(void *data)
{
    this->visible = true;

    if (!sceneInfo->inEditor) {
        if (data)
            this->type = (TitleCardTypes)(VOID_TO_INT(data));
        this->active    = ACTIVE_NORMAL;
        this->drawGroup = 6;

        switch (this->type) {
            case TITLECARD_FADEUP:
                this->timer = 384;
                this->state.Set(&TitleCard::State_FadeUp);
                this->stateDraw.Set(&TitleCard::Draw_GreyscaleBG);
                this->drawFX = FX_SCALE;
                this->animator.SetAnimation(&sVars->aniFrames, 5, true, 0);
                sVars->actID = this->actID;
                break;
            case TITLECARD_SIDEBAR:
                this->position.x = TO_FIXED(screenInfo->size.x - 116);
                this->position.y = TO_FIXED(-512);
                this->state.Set(&TitleCard::State_SideBar);
                this->animator.SetAnimation(&sVars->aniFrames, 3, true, 0);
                break;
            case TITLECARD_FIRSTWORD:
                this->position.x = TO_FIXED(-538);
                this->position.y = TO_FIXED(32);
                this->state.Set(&TitleCard::State_Word);
                this->stateDraw.Set(&TitleCard::Draw_Word);
                this->animator.SetAnimation(&sVars->aniFrames, 0, true, 0);
                break;
            case TITLECARD_SECONDWORD:
                this->position.x = TO_FIXED(-602);
                this->position.y = TO_FIXED(90);
                this->state.Set(&TitleCard::State_Word);
                this->stateDraw.Set(&TitleCard::Draw_Word);
                this->animator.SetAnimation(&sVars->aniFrames, 0, true, 0);
                break;
            case TITLECARD_ZONE:
                this->position.x = TO_FIXED(screenInfo->size.x * 2);
                this->position.y = TO_FIXED(146);
                this->drawFX     = FX_SCALE;
                this->state.Set(&TitleCard::State_Zone);
                this->animator.SetAnimation(&sVars->aniFrames, 1, true, 0);
                break;
            case TITLECARD_ACTBALL:
                this->position.x = TO_FIXED(-152);
                this->position.y = TO_FIXED(-896);
                this->rotation   = 80;
                this->drawFX     = FX_ROTATE;
                this->state.Set(&TitleCard::State_ActBall);
                this->animator.SetAnimation(&sVars->aniFrames, 2, true, sVars->actID);
                break;
            case TITLECARD_VERTTEXT1:
                this->position.x = TO_FIXED(24);
                this->position.y = TO_FIXED(0);
                this->inkEffect  = INK_BLEND;
                this->state.Set(&TitleCard::State_VertText_Down);
                this->stateDraw.Set(&TitleCard::Draw_VertText);
                this->animator.SetAnimation(&sVars->aniFrames, 4, true, 0);
                break;
            case TITLECARD_VERTTEXT2:
                this->position.x = TO_FIXED(screenInfo->size.x - 160);
                this->position.y = TO_FIXED(-256);
                this->inkEffect  = INK_BLEND;
                this->state.Set(&TitleCard::State_VertText_Up);
                this->stateDraw.Set(&TitleCard::Draw_VertText);
                this->animator.SetAnimation(&sVars->aniFrames, 4, true, 0);
                break;
        }
    }
}

void TitleCard::StageLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/TitleCard.bin", SCOPE_GLOBAL);

    paletteBank[0].Copy(1, 80, 80, 16);

#if RETRO_REV02
    uint16 *tintTable = sVars->tintLookupTable;
#else
    uint16 *tintTable = Palette::GetTintLookupTable();
#endif
    // This is the same tint code from RSDKv5 REV01
    // I could modify it so the tint matches Nexus better but I'm too dumb to know how to do that LOL
    for (int32 i = 0; i < 0x10000; ++i) {
        int32 tintValue = (((uint32)i & 0x1F) + ((i >> 6) & 0x1F) + (((uint16)i >> 11) & 0x1F)) / 3 + 6;
        tintTable[i]    = 0x841 * MIN(0x1F, tintValue);
    }
#if RETRO_REV02
    Palette::SetTintLookupTable(sVars->tintLookupTable);
#endif

    // SetBlendTable(128, 1, 16, 80);

    foreach_all(TitleCard, titleCard) { titleCard->Copy(RSDK_GET_ENTITY_GEN(SLOT_TITLECARD), true); }
}

void TitleCard::State_FadeUp(void)
{
    SET_CURRENT_STATE();

    foreach_active(Player, player)
    {
        player->controlMode = Player::CONTROLMODE_NONE;
        player->up          = false;
        player->down        = false;
        player->left        = false;
        player->right       = false;
        player->jumpPress   = false;
        player->jumpHold    = false;
    }

    if (this->timer) {
        if (this->timer == 256) {
            Music::Play(Music::TRACK_STAGE);

            GameObject::Reset(SLOT_TITLECARD_VERTTEXT1, sVars->classID, TITLECARD_VERTTEXT1);
            GameObject::Reset(SLOT_TITLECARD_VERTTEXT2, sVars->classID, TITLECARD_VERTTEXT2);
            GameObject::Reset(SLOT_TITLECARD_SIDEBAR, sVars->classID, TITLECARD_SIDEBAR);
            GameObject::Reset(SLOT_TITLECARD_FIRSTWORD, sVars->classID, TITLECARD_FIRSTWORD);
            GameObject::Reset(SLOT_TITLECARD_SECONDWORD, sVars->classID, TITLECARD_SECONDWORD);
            GameObject::Reset(SLOT_TITLECARD_ZONE, sVars->classID, TITLECARD_ZONE);
            GameObject::Reset(SLOT_TITLECARD_ACTBALL, sVars->classID, TITLECARD_ACTBALL);

            TitleCard *sideBar  = RSDK_GET_ENTITY(SLOT_TITLECARD_SIDEBAR, TitleCard);
            TitleCard *actBall  = RSDK_GET_ENTITY(SLOT_TITLECARD_ACTBALL, TitleCard);
            actBall->position.x = sideBar->position.x - TO_FIXED(460);
        }

        this->timer -= 8;
    }
    else
        this->state.Set(StateMachine_None);
}

void TitleCard::State_SideBar(void)
{
    SET_CURRENT_STATE();

    if (this->position.y < 0)
        this->position.y += TO_FIXED(16);
}

void TitleCard::State_Word(void)
{
    SET_CURRENT_STATE();

    TitleCard *sideBar   = RSDK_GET_ENTITY(SLOT_TITLECARD_SIDEBAR, TitleCard);
    TitleCard *titleCard = RSDK_GET_ENTITY(SLOT_TITLECARD, TitleCard);

    int32 offset = sideBar->position.x - TO_FIXED(titleCard->offset);
    if (this->position.x < offset) {
        this->position.x += TO_FIXED(16);
        if (this->position.x > offset)
            this->position.x = offset;
    }
}

void TitleCard::State_Zone(void)
{
    SET_CURRENT_STATE();

    int32 targetPos = TO_FIXED(screenInfo->size.x - 148);
    if (this->position.x > targetPos) {
        this->position.x -= TO_FIXED(8);
        if (this->position.x < targetPos)
            this->position.x = targetPos;
        this->scale.x = (FROM_FIXED(this->position.x - targetPos) << 4) + 512;
        this->scale.y = this->scale.x;
    }
}

void TitleCard::State_ActBall(void)
{
    SET_CURRENT_STATE();

    if (this->timer < 2) {
        this->rotation = (this->rotation + 16) & 511;

        this->velocity.y += 24576;
        this->position.y += this->velocity.y;
        if (this->position.y > TO_FIXED(136)) {
            this->position.y = TO_FIXED(136);
            this->velocity.y = -this->velocity.y / 3;
            if (++this->timer == 2) {
                TitleCard *titleCard = RSDK_GET_ENTITY(SLOT_TITLECARD, TitleCard);
                titleCard->state.Set(&TitleCard::State_ColourCircle);
                titleCard->stateDraw.Set(&TitleCard::Draw_ColourCircle);
                titleCard->scale     = { 0, 0 };
                titleCard->inkEffect = INK_TINT;
            }
        }
        this->position.x += TO_FIXED(4);
    }
}

void TitleCard::State_VertText_Down(void)
{
    SET_CURRENT_STATE();

    this->position.x += TO_FIXED(1);
    this->position.y += TO_FIXED(2);
    if (this->position.y > 0)
        this->position.y -= TO_FIXED(516);
}

void TitleCard::State_VertText_Up(void)
{
    SET_CURRENT_STATE();

    this->position.x -= TO_FIXED(1);
    this->position.y -= TO_FIXED(2);
    if (this->position.y < TO_FIXED(-516))
        this->position.y += TO_FIXED(516);
}

void TitleCard::State_ColourCircle(void)
{
    SET_CURRENT_STATE();

    if (this->scale.x < 1408) {
        this->scale.x += 12;
        this->scale.y = this->scale.x;
    }
    else {
        RSDK_GET_ENTITY(SLOT_TITLECARD_SIDEBAR, TitleCard)->state.Set(&TitleCard::State_SideBar_Exit);
        RSDK_GET_ENTITY(SLOT_TITLECARD_FIRSTWORD, TitleCard)->state.Set(&TitleCard::State_FirstWord_Exit);
        RSDK_GET_ENTITY(SLOT_TITLECARD_SECONDWORD, TitleCard)->state.Set(&TitleCard::State_SecondWord_Exit);
        RSDK_GET_ENTITY(SLOT_TITLECARD_ZONE, TitleCard)->state.Set(&TitleCard::State_Zone_Exit);
        RSDK_GET_ENTITY(SLOT_TITLECARD_ACTBALL, TitleCard)->state.Set(&TitleCard::State_ActBall_Exit);
    }
}

void TitleCard::State_SideBar_Exit(void)
{
    SET_CURRENT_STATE();

    RSDK_GET_ENTITY_GEN(SLOT_TITLECARD_VERTTEXT1)->position.x += TO_FIXED(7);
    RSDK_GET_ENTITY_GEN(SLOT_TITLECARD_VERTTEXT2)->position.x -= TO_FIXED(7);
    if (this->position.y > TO_FIXED(-192))
        this->position.y -= TO_FIXED(16);
}

void TitleCard::State_FirstWord_Exit(void)
{
    SET_CURRENT_STATE();

    int32 offset = TO_FIXED(screenInfo->center.x - RSDK_GET_ENTITY(SLOT_TITLECARD, TitleCard)->offset - 64);
    if (RSDK_GET_ENTITY_GEN(SLOT_TITLECARD_SECONDWORD)->position.x < offset)
        this->position.x -= TO_FIXED(16);
}

void TitleCard::State_SecondWord_Exit(void)
{
    SET_CURRENT_STATE();
    this->position.x -= TO_FIXED(16);
}

void TitleCard::State_Zone_Exit(void)
{
    SET_CURRENT_STATE();

    this->position.x += TO_FIXED(8);
    if (this->position.x > TO_FIXED(screenInfo->size.x + 64)) {
        paletteBank[0].Copy(2, 80, 80, 16);
        // SetBlendTable(128, 1, 16, 80);

        foreach_active(Player, player) player->controlMode = Player::CONTROLMODE_NORMAL;

        for (int32 e = SLOT_TITLECARD; e < SLOT_TITLECARD_ACTBALL; e++) RSDK_GET_ENTITY_GEN(e)->Destroy();
        GameObject::Reset(SLOT_HUD, $(HUD)->classID, 1);
    }
}

void TitleCard::State_ActBall_Exit(void)
{
    SET_CURRENT_STATE();

    this->rotation = (this->rotation + 32) & 511;
    this->position.x += 524288;
}

void TitleCard::Draw_GreyscaleBG(void)
{
    SET_CURRENT_STATE();

    Graphics::DrawRect(0, 0, screenInfo->size.x, screenInfo->size.y, 0x000000, 0xFF, INK_TINT, true);
    if (this->timer)
        Graphics::FillScreen(0x000000, this->timer, this->timer, this->timer);
}

void TitleCard::Draw_ColourCircle(void)
{
    SET_CURRENT_STATE();

    int32 scaleTemp0 = (128 * this->scale.x) >> 9;
    int32 scaleTemp5 = (256 * this->scale.x) >> 9;
    int32 scaleTemp1 = screenInfo->center.x - scaleTemp0;
    int32 scaleTemp2 = scaleTemp1 + scaleTemp5;
    int32 scaleTemp3 = screenInfo->center.y - scaleTemp0;
    int32 scaleTemp4 = scaleTemp3 + scaleTemp5;

    Graphics::DrawRect(0, 0, scaleTemp1, screenInfo->size.y, 0x000000, 0xFF, INK_TINT, true);
    Graphics::DrawRect(scaleTemp2, 0, screenInfo->size.x, screenInfo->size.y, 0x000000, 0xFF, INK_TINT, true);
    Graphics::DrawRect(scaleTemp1, 0, scaleTemp5, scaleTemp3, 0x000000, 0xFF, INK_TINT, true);
    Graphics::DrawRect(scaleTemp1, scaleTemp4, scaleTemp5, screenInfo->size.y, 0x000000, 0xFF, INK_TINT, true);

    Vector2 pos;
    pos.x = TO_FIXED(screenInfo->center.x);
    pos.y = TO_FIXED(screenInfo->center.y);
    this->animator.DrawSprite(&pos, true);
}

void TitleCard::Draw_Word(void)
{
    SET_CURRENT_STATE();

    Vector2 pos = this->position;
    String *word;
    if (this->type == TITLECARD_FIRSTWORD)
        word = &RSDK_GET_ENTITY(SLOT_TITLECARD, TitleCard)->word1;
    else
        word = &RSDK_GET_ENTITY(SLOT_TITLECARD, TitleCard)->word2;

    if (!word || !word->length)
        return;

    for (int32 c = 0; c < word->length; c++) {
        for (int32 f = 0; f < this->animator.frameCount; f++) {
            this->animator.frameID = f;
            if (this->animator.GetFrameID() == word->chars[c])
                break;
        }
        if (this->type == TITLECARD_FIRSTWORD && word->chars[c] >= 'a' && word->chars[c] <= 'z') {
            pos.y += TO_FIXED(13);
            this->animator.DrawSprite(&pos, true);
            pos.y -= TO_FIXED(13);
        }
        else
            this->animator.DrawSprite(&pos, true);
        pos.x += TO_FIXED(this->animator.GetFrame(sVars->aniFrames)->width - 2);
    }
}

void TitleCard::Draw_VertText(void)
{
    SET_CURRENT_STATE();

    Vector2 pos = this->position;

    this->animator.frameID = 0;
    this->animator.DrawSprite(&pos, true);
    this->animator.frameID = 1;
    this->animator.DrawSprite(&pos, true);

    pos.y += TO_FIXED(516);

    this->animator.frameID = 0;
    this->animator.DrawSprite(&pos, true);
    this->animator.frameID = 1;
    this->animator.DrawSprite(&pos, true);
}

#if GAME_INCLUDE_EDITOR
void TitleCard::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 1);
    this->animator.DrawSprite(NULL, false);
}

void TitleCard::EditorLoad(void)
{
    sVars->aniFrames.Load("NexusGlobals/Editor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR($(TitleCard), actID);
    RSDK_ENUM_VAR("Act 1", ACT_1);
    RSDK_ENUM_VAR("Act 2", ACT_2);
    RSDK_ENUM_VAR("Act 3", ACT_3);
}
#endif

#if RETRO_REV0U
void TitleCard::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(TitleCard); }
#endif

void TitleCard::Serialize(void)
{
    RSDK_EDITABLE_VAR(TitleCard, VAR_STRING, word1);
    RSDK_EDITABLE_VAR(TitleCard, VAR_STRING, word2);
    RSDK_EDITABLE_VAR(TitleCard, VAR_UINT8, offset);
    RSDK_EDITABLE_VAR(TitleCard, VAR_ENUM, actID);
}

} // namespace GameLogic
