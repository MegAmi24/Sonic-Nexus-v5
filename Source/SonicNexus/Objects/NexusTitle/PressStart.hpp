#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct PressStart : RSDK::GameObject::Entity {

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
        RSDK::SoundFX sfxEnter;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::Animator animator;
    RSDK::StateMachine<PressStart> state;
    int32 timer;
    int32 fadeTimer;

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

    void State_AwaitStart(void);
    void State_FadeToBlack(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(PressStart);
};
} // namespace GameLogic
