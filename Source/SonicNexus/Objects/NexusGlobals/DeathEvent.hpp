#pragma once

#include "SonicNexus.hpp"

namespace GameLogic
{

struct DeathEvent : RSDK::GameObject::Entity {

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

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    DeathEventTypes type;
    RSDK::StateMachine<DeathEvent> state;
    RSDK::Animator animator;
    RSDK::Vector2 leftPos;
    RSDK::Vector2 rightPos;
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
