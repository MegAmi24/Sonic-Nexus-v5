#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct Intro : RSDK::GameObject::Entity {

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
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::StateMachine<Intro> state;
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

    void State_LoadTheVideo(void);
    void State_FadeToWhite(void);

    static bool32 VideoSkipCB(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Intro);
};
} // namespace GameLogic
