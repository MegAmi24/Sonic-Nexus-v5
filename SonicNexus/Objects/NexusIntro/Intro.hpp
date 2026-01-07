#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct Intro : GameObject::Entity {

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
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    StateMachine<Intro> state;
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
