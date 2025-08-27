#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct ActFinish : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : GameObject::Static {
        SpriteAnimation aniFrames;
        SoundFX sfxScoreAdd;
        SoundFX sfxScoreTotal;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    StateMachine<ActFinish> state;
    Animator gotThroughAnim;
    Animator actNumAnim;
    Animator resultsAnim;
    Animator numbersAnim;
    Vector2 line0Pos;
    Vector2 line1Pos;
    Vector2 line2Pos;
    Vector2 line3Pos;
    Vector2 line4Pos;
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
