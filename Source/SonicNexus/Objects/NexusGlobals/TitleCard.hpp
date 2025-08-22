#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct TitleCard : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum TitleCardTypes {
        TITLECARD_FADEUP,
        TITLECARD_SIDEBAR,
        TITLECARD_FIRSTWORD,
        TITLECARD_SECONDWORD,
        TITLECARD_ZONE,
        TITLECARD_ACTBALL,
        TITLECARD_VERTTEXT1,
        TITLECARD_VERTTEXT2,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
#if RETRO_REV02
        uint16 tintLookupTable[0x10000];
#endif
        uint8 actID;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    TitleCardTypes type;
    RSDK::Animator animator;
    RSDK::StateMachine<TitleCard> state;
    RSDK::StateMachine<TitleCard> stateDraw;
    RSDK::String word1;
    RSDK::String word2;
    ActIDs actID;
    uint8 offset;
    int32 timer;

    // ==============================
    // EVENTS
    // ==============================

    void Create(void *data);
    void Draw();
    void Update();
    void LateUpdate();

    static void StaticUpdate();
    static void StageLoad();
#if RETRO_REV0U
    static void StaticLoad(Static *sVars);
#endif
    static void Serialize();

#if GAME_INCLUDE_EDITOR
    static void EditorLoad();
    void EditorDraw();
#endif

    // ==============================
    // FUNCTIONS
    // ==============================

    void State_FadeUp(void);
    void State_SideBar(void);
    void State_Word(void);
    void State_Zone(void);
    void State_ActBall(void);
    void State_VertText_Down(void);
    void State_VertText_Up(void);
    void State_ColourCircle(void);
    void State_SideBar_Exit(void);
    void State_FirstWord_Exit(void);
    void State_SecondWord_Exit(void);
    void State_Zone_Exit(void);
    void State_ActBall_Exit(void);

    void Draw_GreyscaleBG(void);
    void Draw_ColourCircle(void);
    void Draw_Word(void);
    void Draw_VertText(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(TitleCard);
};
} // namespace GameLogic
