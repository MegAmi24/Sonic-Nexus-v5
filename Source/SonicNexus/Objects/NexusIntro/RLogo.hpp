#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct RLogo : GameObject::Entity {

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
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    StateMachine<RLogo> state;
    int32 timer;
    Animator animator;

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

    void State_FadeFromBlack(void);
    void State_Wait(void);
    void State_FadeToBlack(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(RLogo);
};
} // namespace GameLogic
