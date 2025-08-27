#pragma once

#include "SonicNexus.hpp"

using namespace RSDK;

namespace GameLogic
{

struct DeathEvent : GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum DeathEventTypes {
        DEATHEVENT_GAMEOVER,
        DEATHEVENT_TIMEOVER,
        DEATHEVENT_FADETOBLACK,
    };

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
    DeathEventTypes type;
    StateMachine<DeathEvent> state;
    Animator animator;
    Vector2 leftPos;
    Vector2 rightPos;
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

    void State_GameOver(void); // Also for Time Over
    void State_FadeToBlack(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(DeathEvent);
};
} // namespace GameLogic
