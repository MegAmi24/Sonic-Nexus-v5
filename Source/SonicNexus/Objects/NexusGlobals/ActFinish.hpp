#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct ActFinish : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
        RSDK::SoundFX sfxScoreAdd;
        RSDK::SoundFX sfxScoreTotal;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::StateMachine<ActFinish> state;
    RSDK::Animator gotThroughAnim;
    RSDK::Animator actNumAnim;
    RSDK::Animator resultsAnim;
    RSDK::Animator numbersAnim;
    RSDK::Vector2 line0Pos;
    RSDK::Vector2 line1Pos;
    RSDK::Vector2 line2Pos;
    RSDK::Vector2 line3Pos;
    RSDK::Vector2 line4Pos;
    int32 timer;
    int32 timeBonus;
    int32 ringBonus;
    int32 total;

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

    void State_MoveIn(void);
    void State_Wait(void);
    void State_AddEverying(void);
    void State_WaitSomeMore(void);
    void State_FadeOut(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(ActFinish);
};
} // namespace GameLogic
